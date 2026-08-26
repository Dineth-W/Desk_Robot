#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "call.h"
#include "bluetooth.h"

static const char *TAG = "MAIN";

void app_main(void) {
    ESP_LOGI(TAG, "Starting Application...");

    // Initialize bluetooth stack first
    ESP_ERROR_CHECK(bluetooth_init());

    // Initialize call module (now returns esp_err_t)
    ESP_ERROR_CHECK(call_init());

    // Example call control logic
    if (call_get_state() == CALL_INCOMING) {
        esp_err_t r = call_answer();
        if (r != ESP_OK) {
            ESP_LOGE(TAG, "Failed to answer call");
        }
    } else if (call_get_state() == CALL_ACTIVE) {
        esp_err_t r = call_end();
        if (r != ESP_OK) {
            ESP_LOGE(TAG, "Failed to end call");
        }
    } else {
        esp_err_t r = call_reject();
        (void)r;
    }
}
