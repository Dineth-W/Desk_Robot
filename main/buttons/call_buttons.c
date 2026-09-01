#include "call_buttons.h"

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_hf_client_api.h"

static const char *TAG = "CALL_BUTTONS";

#define BUTTON_DEBOUNCE_MS       50
#define BUTTON_SCAN_MS           20

static void call_buttons_task(void *arg)
{
    bool answer_was_pressed = false;
    bool end_was_pressed = false;

    while (1)
    {
        bool answer_pressed =
            (gpio_get_level(CALL_ANSWER_BUTTON_GPIO) == 0);

        bool end_pressed =
            (gpio_get_level(CALL_END_BUTTON_GPIO) == 0);

        /*
         * ANSWER BUTTON
         *
         * GPIO32 LOW -> answer call
         */
        if (answer_pressed && !answer_was_pressed)
        {
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));

            if (gpio_get_level(CALL_ANSWER_BUTTON_GPIO) == 0)
            {
                ESP_LOGI(TAG, "ANSWER button pressed");

                esp_err_t ret = esp_hf_client_answer_call();

                if (ret == ESP_OK)
                {
                    ESP_LOGI(TAG, "Answer command sent");
                }
                else
                {
                    ESP_LOGW(TAG,
                             "Answer command failed: %s",
                             esp_err_to_name(ret));
                }

                answer_was_pressed = true;
            }
        }

        /*
         * END / REJECT BUTTON
         *
         * GPIO13 LOW -> reject/end call
         */
        if (end_pressed && !end_was_pressed)
        {
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));

            if (gpio_get_level(CALL_END_BUTTON_GPIO) == 0)
            {
                ESP_LOGI(TAG, "END/REJECT button pressed");

                esp_err_t ret = esp_hf_client_reject_call();

                if (ret == ESP_OK)
                {
                    ESP_LOGI(TAG, "Reject/end command sent");
                }
                else
                {
                    ESP_LOGW(TAG,
                             "Reject/end command failed: %s",
                             esp_err_to_name(ret));
                }

                end_was_pressed = true;
            }
        }

        /*
         * Wait until buttons are released before
         * allowing another command.
         */
        if (!answer_pressed)
        {
            answer_was_pressed = false;
        }

        if (!end_pressed)
        {
            end_was_pressed = false;
        }

        vTaskDelay(pdMS_TO_TICKS(BUTTON_SCAN_MS));
    }
}


void call_buttons_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask =
            (1ULL << CALL_ANSWER_BUTTON_GPIO) |
            (1ULL << CALL_END_BUTTON_GPIO),

        .mode = GPIO_MODE_INPUT,

        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,

        .intr_type = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ESP_LOGI(TAG,
             "Call buttons initialized: ANSWER=GPIO32, END=GPIO13");

    xTaskCreate(
        call_buttons_task,
        "call_buttons",
        4096,
        NULL,
        5,
        NULL
    );
}