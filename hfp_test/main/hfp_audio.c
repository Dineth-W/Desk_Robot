#include "hfp_audio.h"

#include <string.h>
#include <stdint.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"
#include "driver/i2s_std.h"
#include "esp_hf_client_api.h"

#define TAG "HFP_AUDIO"

/*
 * HFP narrow-band audio:
 *   sample rate = 8 kHz
 *   format     = signed 16-bit PCM
 *   channels   = mono
 *
 * Current HFP packets observed from the working project are 120 bytes.
 * 120 bytes = 60 samples = 7.5 ms at 8 kHz.
 */
#define HFP_SAMPLE_RATE_HZ       8000
#define HFP_SAMPLE_BYTES         2
#define HFP_RX_RING_SIZE         8192
#define HFP_TX_RING_SIZE         8192

/*
 * I2S is configured as 32-bit stereo:
 *   BCLK = 8 kHz * 32 bits * 2 slots = 512 kHz
 *
 * This gives the INMP441 the required 64 SCK cycles per WS frame.
 */
#define I2S_DMA_FRAME_NUM        64
#define I2S_DMA_DESC_NUM         8

static i2s_chan_handle_t s_i2s_tx = NULL;
static i2s_chan_handle_t s_i2s_rx = NULL;

static RingbufHandle_t s_hfp_to_speaker = NULL;
static RingbufHandle_t s_microphone_to_hfp = NULL;

static volatile bool s_audio_active = false;
static bool s_initialized = false;

static void speaker_task(void *arg)
{
    (void)arg;

    /*
     * Maximum normal HFP packet is small. Use a bounded local buffer so
     * this task never allocates audio buffers dynamically.
     */
    int32_t i2s_stereo[240]; /* 120 frames max */
    uint8_t hfp_pcm[240];       /* receive buffer for HFP PCM */

    while (true) {
        if (!s_hfp_to_speaker) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        size_t len = 0;
        uint8_t *data = (uint8_t *)xRingbufferReceiveUpTo(
            s_hfp_to_speaker,
            &len,
            pdMS_TO_TICKS(20),
            sizeof(hfp_pcm));

        if (!data) {
            continue;
        }

        if (!s_audio_active) {
            vRingbufferReturnItem(s_hfp_to_speaker, data);
            continue;
        }

        /*
         * HFP gives 16-bit mono PCM.
         * MAX98357A is fed 32-bit stereo. Put the same sample in both
         * channels and left-shift to occupy the useful high bits.
         */
        size_t samples = len / sizeof(int16_t);
        if (samples > 120) {
            samples = 120;
        }

        const int16_t *pcm = (const int16_t *)data;

        for (size_t i = 0; i < samples; ++i) {
            int32_t sample = ((int32_t)pcm[i]) << 16;
            i2s_stereo[2 * i] = sample;
            i2s_stereo[2 * i + 1] = sample;
        }

        size_t bytes_written = 0;
        esp_err_t err = i2s_channel_write(
            s_i2s_tx,
            i2s_stereo,
            samples * 2 * sizeof(int32_t),
            &bytes_written,
            pdMS_TO_TICKS(100));

        if (err != ESP_OK) {
            ESP_LOGW(TAG, "I2S speaker write failed: %s",
                     esp_err_to_name(err));
        }

        vRingbufferReturnItem(s_hfp_to_speaker, data);
    }
}

static void microphone_task(void *arg)
{
    (void)arg;

    /*
     * 60 frames = 7.5 ms at 8 kHz.
     * Each frame has left + right 32-bit slots.
     */
    int32_t i2s_stereo[120];
    uint8_t hfp_pcm[120];

    while (true) {
        size_t bytes_read = 0;

        esp_err_t err = i2s_channel_read(
            s_i2s_rx,
            i2s_stereo,
            sizeof(i2s_stereo),
            &bytes_read,
            portMAX_DELAY);

        if (err != ESP_OK || bytes_read == 0) {
            if (err != ESP_ERR_TIMEOUT) {
                ESP_LOGW(TAG, "I2S microphone read failed: %s",
                         esp_err_to_name(err));
            }
            continue;
        }

        if (!s_audio_active || !s_microphone_to_hfp) {
            continue;
        }

        size_t frames = bytes_read / (2 * sizeof(int32_t));
        if (frames > 60) {
            frames = 60;
        }

        /*
         * INMP441 outputs a 24-bit signed value in the 32-bit I2S slot.
         * With the Philips 32-bit configuration, take the upper 16 bits
         * for the HFP narrow-band PCM stream.
         *
         * L/R = GND selects the left channel, so use every left slot.
         */
        for (size_t i = 0; i < frames; ++i) {
            int32_t mic_left = i2s_stereo[2 * i];
            int16_t pcm16 = (int16_t)(mic_left >> 16);

            hfp_pcm[2 * i] = (uint8_t)(pcm16 & 0xff);
            hfp_pcm[2 * i + 1] = (uint8_t)((uint16_t)pcm16 >> 8);
        }

        size_t len = frames * sizeof(int16_t);

        if (!xRingbufferSend(
                s_microphone_to_hfp,
                hfp_pcm,
                len,
                0)) {
            ESP_LOGW(TAG, "Microphone HFP queue full; dropping %u bytes",
                     (unsigned)len);
            continue;
        }

        /*
         * Wake the Bluetooth HFP layer. The outgoing callback itself
         * remains non-blocking and only copies already-prepared PCM.
         */
        esp_hf_client_outgoing_data_ready();
    }
}

esp_err_t hfp_audio_init(void)
{
    if (s_initialized) {
        return ESP_OK;
    }

    s_hfp_to_speaker = xRingbufferCreate(
        HFP_RX_RING_SIZE,
        RINGBUF_TYPE_BYTEBUF);

    s_microphone_to_hfp = xRingbufferCreate(
        HFP_TX_RING_SIZE,
        RINGBUF_TYPE_BYTEBUF);

    if (!s_hfp_to_speaker || !s_microphone_to_hfp) {
        ESP_LOGE(TAG, "Failed to allocate HFP audio ring buffers");
        return ESP_ERR_NO_MEM;
    }

    /*
     * Allocate TX and RX on the same I2S port. Matching clock/frame
     * configuration makes them a full-duplex pair.
     */
    i2s_chan_config_t chan_cfg =
        I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

    chan_cfg.dma_desc_num = I2S_DMA_DESC_NUM;
    chan_cfg.dma_frame_num = I2S_DMA_FRAME_NUM;

    esp_err_t err = i2s_new_channel(
        &chan_cfg,
        &s_i2s_tx,
        &s_i2s_rx);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2s_new_channel failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(HFP_SAMPLE_RATE_HZ),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
            I2S_DATA_BIT_WIDTH_32BIT,
            I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = GPIO_NUM_25,
            .ws = GPIO_NUM_26,
            .dout = GPIO_NUM_27,
            .din = GPIO_NUM_33,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    err = i2s_channel_init_std_mode(s_i2s_tx, &std_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S TX init failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    err = i2s_channel_init_std_mode(s_i2s_rx, &std_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S RX init failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    err = i2s_channel_enable(s_i2s_tx);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S TX enable failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    err = i2s_channel_enable(s_i2s_rx);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2S RX enable failed: %s",
                 esp_err_to_name(err));
        return err;
    }

    BaseType_t ok = xTaskCreate(
        speaker_task,
        "hfp_spk",
        4096,
        NULL,
        6,
        NULL);

    if (ok != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    ok = xTaskCreate(
        microphone_task,
        "hfp_mic",
        4096,
        NULL,
        6,
        NULL);

    if (ok != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    s_initialized = true;

    ESP_LOGI(TAG,
             "I2S audio ready: 8 kHz, 32-bit stereo, BCLK=25 WS=26 OUT=27 IN=33");

    return ESP_OK;
}

static void drain_ringbuffer(RingbufHandle_t rb)
{
    if (!rb) {
        return;
    }

    while (true) {
        size_t len = 0;
        uint8_t *data = (uint8_t *)xRingbufferReceiveUpTo(
            rb,
            &len,
            0,
            512);

        if (!data) {
            break;
        }

        vRingbufferReturnItem(rb, data);
    }
}

void hfp_audio_start(void)
{
    if (!s_initialized) {
        ESP_LOGE(TAG, "hfp_audio_start called before init");
        return;
    }

    drain_ringbuffer(s_hfp_to_speaker);
    drain_ringbuffer(s_microphone_to_hfp);

    s_audio_active = true;

    ESP_LOGI(TAG, "HFP PCM audio ACTIVE");
}

void hfp_audio_stop(void)
{
    /*
     * Stop accepting HFP audio immediately.
     */
    s_audio_active = false;

    /*
     * Stop I2S TX first so the MAX98357A cannot continue
     * playing data that is already sitting in the DMA buffer.
     */
    if (s_i2s_tx) {
        esp_err_t err = i2s_channel_disable(s_i2s_tx);

        if (err != ESP_OK) {
            ESP_LOGW(TAG,
                     "I2S TX disable failed: %s",
                     esp_err_to_name(err));
        }
    }

    /*
     * Stop microphone RX as well.
     */
    if (s_i2s_rx) {
        esp_err_t err = i2s_channel_disable(s_i2s_rx);

        if (err != ESP_OK) {
            ESP_LOGW(TAG,
                     "I2S RX disable failed: %s",
                     esp_err_to_name(err));
        }
    }

    /*
     * Clear any HFP audio that arrived just before the
     * Bluetooth SCO connection disappeared.
     */
    drain_ringbuffer(s_hfp_to_speaker);
    drain_ringbuffer(s_microphone_to_hfp);

    /*
     * Re-enable the channels for the next call.
     * We keep the I2S hardware initialized; we only stop
     * the channels between calls.
     */
    if (s_i2s_tx) {
        esp_err_t err = i2s_channel_enable(s_i2s_tx);

        if (err != ESP_OK) {
            ESP_LOGW(TAG,
                     "I2S TX re-enable failed: %s",
                     esp_err_to_name(err));
        }
    }

    if (s_i2s_rx) {
        esp_err_t err = i2s_channel_enable(s_i2s_rx);

        if (err != ESP_OK) {
            ESP_LOGW(TAG,
                     "I2S RX re-enable failed: %s",
                     esp_err_to_name(err));
        }
    }

    ESP_LOGI(TAG, "HFP PCM audio INACTIVE");
}

bool hfp_audio_put_rx_pcm(const uint8_t *data, size_t len)
{
    if (!s_audio_active || !data || len == 0 || !s_hfp_to_speaker) {
        return false;
    }

    return xRingbufferSend(
        s_hfp_to_speaker,
        (void *)data,
        len,
        0) == pdTRUE;
}

bool hfp_audio_get_tx_pcm(uint8_t *data, size_t len)
{
    if (!s_audio_active || !data || len == 0 || !s_microphone_to_hfp) {
        return false;
    }

    size_t item_size = 0;

    uint8_t *item = (uint8_t *)xRingbufferReceiveUpTo(
        s_microphone_to_hfp,
        &item_size,
        0,
        len);

    if (!item) {
        return false;
    }

    if (item_size != len) {
        vRingbufferReturnItem(s_microphone_to_hfp, item);
        return false;
    }

    memcpy(data, item, len);
    vRingbufferReturnItem(s_microphone_to_hfp, item);

    return true;
}
