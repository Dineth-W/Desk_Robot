#include "config.h"
#include "display.h"
#include "clock.h"
#include "face.h"
#include "alarm.h"
#include "rtc.h"
#include "screen.h"

#include "driver/gpio.h"

#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG =
    "DESK_ROBOT";


/* =========================================================
 * BUTTON INITIALIZATION
 * ========================================================= */

static void button_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask =
            (1ULL << BTN_1),

        .mode =
            GPIO_MODE_INPUT,

        .pull_up_en =
            GPIO_PULLUP_ENABLE,

        .pull_down_en =
            GPIO_PULLDOWN_DISABLE,

        .intr_type =
            GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(
        gpio_config(&config)
    );

    ESP_LOGI(
        TAG,
        "Button initialized on GPIO%d",
        BTN_1
    );
}


/* =========================================================
 * APP MAIN
 * ========================================================= */

void app_main(void)
{
    ESP_LOGI(
        TAG,
        "Desk Robot starting..."
    );


    /* -----------------------------------------------------
     * DISPLAY
     * ----------------------------------------------------- */

    esp_err_t err =
        display_init();

    if (err != ESP_OK) {

        ESP_LOGE(
            TAG,
            "Display initialization failed."
        );

        /*
         * Keep the firmware running so
         * serial diagnostics remain available.
         */

    } else {

        display_show_splash();

        vTaskDelay(
            pdMS_TO_TICKS(1500)
        );
    }


    /* -----------------------------------------------------
     * SYSTEM MODULES
     * ----------------------------------------------------- */

    screen_init();

    clock_init();

    face_init();

    alarm_init();

    button_init();


    /* -----------------------------------------------------
     * RTC
     * ----------------------------------------------------- */

    err = rtc_init();

    if (err == ESP_OK) {

        ESP_LOGI(
            TAG,
            "DS3231 detected."
        );

    } else {

        ESP_LOGW(
            TAG,
            "DS3231 not available."
        );
    }


    /* -----------------------------------------------------
     * INITIAL SCREEN
     * ----------------------------------------------------- */

    if (oled != NULL) {

        draw_clock();

        change_screen(
            SCREEN_CLOCK
        );
    }


    ESP_LOGI(
        TAG,
        "Desk Robot started."
    );


    /* =====================================================
     * MAIN LOOP
     * ===================================================== */

    int previous_button_state = 1;

    while (1) {

        /* -------------------------------------------------
         * BUTTON
         * ------------------------------------------------- */

        int button_state =
            gpio_get_level(BTN_1);

        if (previous_button_state == 1 &&
            button_state == 0) {

            ESP_LOGI(
                TAG,
                "Button pressed."
            );

            set_expression(
                FACE_HAPPY
            );

            change_screen(
                SCREEN_FACE
            );
        }

        previous_button_state =
            button_state;


        /* -------------------------------------------------
         * CLOCK
         * ------------------------------------------------- */

        update_clock();


        /* -------------------------------------------------
         * FACE
         * ------------------------------------------------- */

        update_face();


        /* -------------------------------------------------
         * ALARMS
         * ------------------------------------------------- */

        update_alarm();


        /* -------------------------------------------------
         * LOOP DELAY
         * ------------------------------------------------- */

        vTaskDelay(
            pdMS_TO_TICKS(100)
        );
    }
}