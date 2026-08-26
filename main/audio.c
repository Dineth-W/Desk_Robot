#include "audio.h"
#include "config.h"
#include "driver/i2s_std.h"
#include "esp_log.h"
#include <math.h>

static const char *TAG="AUDIO";
static i2s_chan_handle_t rx;
static i2s_chan_handle_t tx;

esp_err_t audio_init(void)
{
    i2s_chan_config_t chan_cfg=I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0,I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num=6;
    chan_cfg.dma_frame_num=AUDIO_BUFFER_SAMPLES;

    esp_err_t e=i2s_new_channel(&chan_cfg,&tx,&rx);
    if(e!=ESP_OK) return e;

    i2s_std_config_t cfg={
        .clk_cfg=I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg=I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT,I2S_SLOT_MODE_MONO),
        .gpio_cfg={
            .mclk=I2S_GPIO_UNUSED,
            .bclk=I2S_BCLK_GPIO,
            .ws=I2S_WS_GPIO,
            .dout=I2S_SPK_DATA_GPIO,
            .din=I2S_MIC_DATA_GPIO,
            .invert_flags={0}
        }
    };

    e=i2s_channel_init_std_mode(tx,&cfg);
    if(e!=ESP_OK) return e;

    e=i2s_channel_init_std_mode(rx,&cfg);
    if(e!=ESP_OK) return e;

    i2s_channel_enable(tx);
    i2s_channel_enable(rx);

    ESP_LOGI(TAG,"I2S ready: 16 kHz, 32-bit mono, BCLK=%d WS=%d RX=%d TX=%d",
             I2S_BCLK_GPIO,I2S_WS_GPIO,I2S_MIC_DATA_GPIO,I2S_SPK_DATA_GPIO);
    return ESP_OK;
}

esp_err_t audio_read(int32_t *samples,size_t count,size_t *read)
{
    return i2s_channel_read(rx,samples,count*sizeof(int32_t),read,1000);
}

esp_err_t audio_write(const int32_t *samples,size_t count)
{
    size_t written;
    return i2s_channel_write(tx,samples,count*sizeof(int32_t),&written,1000);
}

float audio_rms(const int32_t *s,size_t n)
{
    if(!n) return 0;
    double mean=0;
    for(size_t i=0;i<n;i++) mean += (double)s[i];
    mean/=n;

    double sum=0;
    for(size_t i=0;i<n;i++){
        double x=(double)s[i]-mean;
        sum += x*x;
    }
    return (float)sqrt(sum/n);
}
