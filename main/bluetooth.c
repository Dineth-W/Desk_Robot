#include "bluetooth.h"
#include "esp_log.h"

static const char *TAG="BT";

esp_err_t bluetooth_init(void)
{
    /*
     * Bluetooth HFP is intentionally kept behind this module.
     * The previous project investigation confirmed the ESP-IDF
     * hfp_hf example builds, but real phone-call integration has
     * not yet been merged into the Desk Robot firmware.
     */
    ESP_LOGI(TAG,"Bluetooth module reserved for HFP integration");
    ESP_LOGI(TAG,"Use ESP-IDF hfp_hf example as the integration source later");
    return ESP_OK;
}

void bluetooth_update(void) {}
