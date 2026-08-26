#include "call_audio.h"

#include "esp_log.h"

static const char *TAG = "CALL_AUDIO";

esp_err_t call_audio_init(void)
{
    ESP_LOGI(TAG, "Call audio subsystem initialized");

    /*
     * Later:
     *
     * INMP441
     *      ↓
     * I2S RX
     *      ↓
     * HFP SCO
     *
     * HFP SCO
     *      ↓
     * I2S TX
     *      ↓
     * MAX98357A
     */

    return ESP_OK;
}

esp_err_t call_audio_start(void)
{
    ESP_LOGI(TAG, "Call audio START");

    return ESP_OK;
}

esp_err_t call_audio_stop(void)
{
    ESP_LOGI(TAG, "Call audio STOP");

    return ESP_OK;
}

esp_err_t call_audio_read(
    int32_t *buffer,
    size_t samples,
    size_t *bytes_read)
{
    (void)buffer;
    (void)samples;

    if (bytes_read)
        *bytes_read = 0;

    return ESP_OK;
}

esp_err_t call_audio_write(
    const int32_t *buffer,
    size_t samples,
    size_t *bytes_written)
{
    (void)buffer;
    (void)samples;

    if (bytes_written)
        *bytes_written = 0;

    return ESP_OK;
}