#include "call.h"
#include "bluetooth.h"
#include "esp_log.h"

static const char *TAG = "CALL";
static call_state_t current_call_state = CALL_IDLE;

esp_err_t call_init(void) {
    ESP_LOGI(TAG, "Initializing Call Module");
    current_call_state = CALL_IDLE;
    return ESP_OK;
}

call_state_t call_get_state(void) {
    return current_call_state;
}

esp_err_t call_answer(void) {
    ESP_LOGI(TAG, "Answering Call...");
    current_call_state = CALL_ACTIVE;
    return bluetooth_answer();
}

esp_err_t call_reject(void) {
    ESP_LOGI(TAG, "Rejecting Call...");
    current_call_state = CALL_IDLE;
    return bluetooth_reject();
}

esp_err_t call_end(void) {
    ESP_LOGI(TAG, "Ending Call...");
    current_call_state = CALL_IDLE;
    return bluetooth_hangup();
}