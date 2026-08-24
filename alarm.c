#include "alarm.h"
#include "clock.h"
#include "display.h"
#include "config.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#include <stdio.h>

static const char *TAG = "ALARM";


Alarm alarms[MAX_ALARMS];

int alarmCount = 0;


/* =========================================================
 * BUZZER
 * ========================================================= */

void buzzer_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask =
            (1ULL << BUZZER_PIN),

        .mode =
            GPIO_MODE_OUTPUT,

        .pull_up_en =
            GPIO_PULLUP_DISABLE,

        .pull_down_en =
            GPIO_PULLDOWN_DISABLE,

        .intr_type =
            GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(
        gpio_config(&config)
    );

    buzzer_off();

    ESP_LOGI(
        TAG,
        "Buzzer initialized on GPIO%d",
        BUZZER_PIN
    );
}


void buzzer_on(void)
{
    gpio_set_level(
        BUZZER_PIN,
        1
    );
}


void buzzer_off(void)
{
    gpio_set_level(
        BUZZER_PIN,
        0
    );
}


/*
 * Simple software tone.
 *
 * 2000 Hz = 500 us period
 * 250 us high
 * 250 us low
 */

void buzzer_beep(
    unsigned long duration_ms
)
{
    unsigned long cycles =
        duration_ms * 2UL;

    for (unsigned long i = 0;
         i < cycles;
         i++) {

        buzzer_on();

        esp_rom_delay_us(250);

        buzzer_off();

        esp_rom_delay_us(250);
    }

    buzzer_off();
}


/* =========================================================
 * ALARM INITIALIZATION
 * ========================================================= */

void alarm_init(void)
{
    alarmCount = 0;

    for (int i = 0; i < MAX_ALARMS; i++) {

        alarms[i].hour = 0;
        alarms[i].minute = 0;

        alarms[i].enabled = false;
        alarms[i].triggered = false;
        alarms[i].repeat = false;
    }

    buzzer_init();

    ESP_LOGI(
        TAG,
        "Alarm system initialized."
    );
}


/* =========================================================
 * ADD ALARM
 * ========================================================= */

bool add_alarm(
    int hour,
    int minute,
    bool repeat
)
{
    if (alarmCount >= MAX_ALARMS) {
        return false;
    }

    if (hour < 0 || hour > 23) {
        return false;
    }

    if (minute < 0 || minute > 59) {
        return false;
    }

    alarms[alarmCount].hour = hour;

    alarms[alarmCount].minute = minute;

    alarms[alarmCount].enabled = true;

    alarms[alarmCount].triggered = false;

    alarms[alarmCount].repeat = repeat;

    alarmCount++;

    return true;
}


/* =========================================================
 * REMOVE ALARM
 * ========================================================= */

void remove_alarm(int index)
{
    if (index < 0 ||
        index >= alarmCount) {

        return;
    }

    for (int i = index;
         i < alarmCount - 1;
         i++) {

        alarms[i] = alarms[i + 1];
    }

    alarmCount--;

    alarms[alarmCount].enabled = false;
}


/* =========================================================
 * GET ALARM
 * ========================================================= */

Alarm *get_alarm(int index)
{
    if (index < 0 ||
        index >= alarmCount) {

        return NULL;
    }

    return &alarms[index];
}


/* =========================================================
 * DRAW ALARM SCREEN
 * ========================================================= */

void draw_alarm_screen(
    const Alarm *alarm
)
{
    if (alarm == NULL) {
        return;
    }

    char time_string[8];

    snprintf(
        time_string,
        sizeof(time_string),
        "%02d:%02d",
        alarm->hour,
        alarm->minute
    );

    display_clear();

    display_center_text(
        "ALARM",
        4,
        1
    );

    display_center_text(
        time_string,
        20,
        2
    );

    if (alarm->repeat) {

        display_center_text(
            "REPEAT",
            48,
            1
        );

    } else {

        display_center_text(
            "ONCE",
            48,
            1
        );
    }

    display_refresh();
}


/* =========================================================
 * UPDATE ALARMS
 * ========================================================= */

void update_alarm(void)
{
    struct tm now =
        get_current_time();

    for (int i = 0;
         i < alarmCount;
         i++) {

        Alarm *alarm =
            &alarms[i];

        if (!alarm->enabled) {
            continue;
        }

        /*
         * Alarm fires once when hour/minute match.
         */

        if (now.tm_hour == alarm->hour &&
            now.tm_min == alarm->minute) {

            if (!alarm->triggered) {

                alarm->triggered = true;

                ESP_LOGI(
                    TAG,
                    "Alarm triggered: %02d:%02d",
                    alarm->hour,
                    alarm->minute
                );

                draw_alarm_screen(
                    alarm
                );

                buzzer_beep(250);

                if (!alarm->repeat) {

                    alarm->enabled = false;
                }
            }

        } else {

            /*
             * Allow the alarm to trigger
             * again on the next matching minute.
             */

            alarm->triggered = false;
        }
    }
}