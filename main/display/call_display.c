#include "call_display.h"

#include "esp_log.h"

static const char *TAG = "CALL_DISPLAY";


void call_display_init(void)
{
    ESP_LOGI(
        TAG,
        "Call display initialized"
    );
}


void call_display_idle(void)
{
    ESP_LOGI(
        TAG,
        "[CALL UI] IDLE"
    );
}


void call_display_incoming(
    const char *number
)
{
    ESP_LOGI(
        TAG,
        "================================"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] INCOMING CALL"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] Number: %s",
        number && number[0]
            ? number
            : "Unknown"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] GPIO32 = ANSWER"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] GPIO34 = REJECT"
    );

    ESP_LOGI(
        TAG,
        "================================"
    );
}


void call_display_active(
    const char *number
)
{
    ESP_LOGI(
        TAG,
        "================================"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] CALL ACTIVE"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] Number: %s",
        number && number[0]
            ? number
            : "Unknown"
    );

    ESP_LOGI(
        TAG,
        "[CALL UI] GPIO34 = END"
    );

    ESP_LOGI(
        TAG,
        "================================"
    );
}


void call_display_ended(void)
{
    ESP_LOGI(
        TAG,
        "[CALL UI] CALL ENDED"
    );
}