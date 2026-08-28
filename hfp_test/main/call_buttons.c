#include "call_buttons.h"

#include <stdbool.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_hf_client_api.h"

#define TAG "CALL_BUTTONS"

#define BUTTON_ACTIVE_LEVEL     0
#define BUTTON_DEBOUNCE_MS      50
#define BUTTON_POLL_MS          20

static volatile bool s_incoming_call = false;
static volatile bool s_call_active = false;

static bool s_answer_last = true;
static bool s_end_last = true;

static void call_buttons_task(void *arg)
{
    bool answer_now;
    bool end_now;

    while (true) {

        answer_now =
            (gpio_get_level(CALL_BUTTON_ANSWER_GPIO) ==
             BUTTON_ACTIVE_LEVEL);

        end_now =
            (gpio_get_level(CALL_BUTTON_END_GPIO) ==
             BUTTON_ACTIVE_LEVEL);

        /*
         * ANSWER BUTTON
         *
         * Only answer when an incoming call is ringing.
         */
        if (answer_now && !s_answer_last) {

            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));

            if (gpio_get_level(CALL_BUTTON_ANSWER_GPIO) ==
                BUTTON_ACTIVE_LEVEL) {

                if (s_incoming_call) {

                    ESP_LOGI(TAG, "ANSWER button pressed");

                    esp_err_t ret =
                        esp_hf_client_answer_call();

                    if (ret != ESP_OK) {
                        ESP_LOGE(
                            TAG,
                            "Failed to answer call: %s",
                            esp_err_to_name(ret)
                        );
                    } else {
                        ESP_LOGI(
                            TAG,
                            "Answer command sent"
                        );
                    }
                } else {
                    ESP_LOGI(
                        TAG,
                        "ANSWER ignored - no incoming call"
                    );
                }

                /*
                 * Wait for button release.
                 */
                while (gpio_get_level(CALL_BUTTON_ANSWER_GPIO) ==
                       BUTTON_ACTIVE_LEVEL) {
                    vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_MS));
                }
            }
        }

        /*
         * END BUTTON
         *
         * Rejects an incoming call or terminates
         * an active call.
         */
        if (end_now && !s_end_last) {

            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));

            if (gpio_get_level(CALL_BUTTON_END_GPIO) ==
                BUTTON_ACTIVE_LEVEL) {

                if (s_incoming_call || s_call_active) {

                    ESP_LOGI(TAG, "END/REJECT button pressed");

                    esp_err_t ret =
                        esp_hf_client_reject_call();

                    if (ret != ESP_OK) {
                        ESP_LOGE(
                            TAG,
                            "Failed to reject/end call: %s",
                            esp_err_to_name(ret)
                        );
                    } else {
                        ESP_LOGI(
                            TAG,
                            "Reject/end command sent"
                        );
                    }

                } else {
                    ESP_LOGI(
                        TAG,
                        "END ignored - no active call"
                    );
                }

                /*
                 * Wait for button release.
                 */
                while (gpio_get_level(CALL_BUTTON_END_GPIO) ==
                       BUTTON_ACTIVE_LEVEL) {
                    vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_MS));
                }
            }
        }

        s_answer_last = answer_now;
        s_end_last = end_now;

        vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_MS));
    }
}

esp_err_t call_buttons_init(void)
{
    gpio_config_t answer_config = {
        .pin_bit_mask = (1ULL << CALL_BUTTON_ANSWER_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    /*
     * GPIO34 is input-only and has no internal pull-up.
     *
     * Therefore GPIO34 requires an external 10kΩ pull-up
     * to 3.3V.
     */
    gpio_config_t end_config = {
        .pin_bit_mask = (1ULL << CALL_BUTTON_END_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    esp_err_t ret;

    ret = gpio_config(&answer_config);

    if (ret != ESP_OK) {
        ESP_LOGE(
            TAG,
            "Failed to configure ANSWER GPIO: %s",
            esp_err_to_name(ret)
        );
        return ret;
    }

    ret = gpio_config(&end_config);

    if (ret != ESP_OK) {
        ESP_LOGE(
            TAG,
            "Failed to configure END GPIO: %s",
            esp_err_to_name(ret)
        );
        return ret;
    }

    s_answer_last =
        (gpio_get_level(CALL_BUTTON_ANSWER_GPIO) ==
         BUTTON_ACTIVE_LEVEL);

    s_end_last =
        (gpio_get_level(CALL_BUTTON_END_GPIO) ==
         BUTTON_ACTIVE_LEVEL);

    BaseType_t task_ret = xTaskCreate(
        call_buttons_task,
        "call_buttons",
        3072,
        NULL,
        5,
        NULL
    );

    if (task_ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create button task");
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(
        TAG,
        "Call buttons initialized: ANSWER=GPIO%d, END=GPIO%d",
        CALL_BUTTON_ANSWER_GPIO,
        CALL_BUTTON_END_GPIO
    );

    return ESP_OK;
}

void call_buttons_set_state(bool incoming, bool active)
{
    s_incoming_call = incoming;
    s_call_active = active;

    ESP_LOGI(
        TAG,
        "Call state updated: incoming=%s active=%s",
        incoming ? "YES" : "NO",
        active ? "YES" : "NO"
    );
}