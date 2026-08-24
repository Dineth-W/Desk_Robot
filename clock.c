#include "clock.h"
#include "display.h"

#include "esp_log.h"

#include <stdio.h>
#include <time.h>

static const char *TAG = "CLOCK";

static struct tm current_time;

static int last_second = -1;


/* =========================================================
 * INITIALIZATION
 * ========================================================= */

void clock_init(void)
{
    time_t now;

    time(&now);

    localtime_r(
        &now,
        &current_time
    );

    last_second = -1;

    ESP_LOGI(
        TAG,
        "Clock initialized: %02d:%02d:%02d",
        current_time.tm_hour,
        current_time.tm_min,
        current_time.tm_sec
    );
}


/* =========================================================
 * GET CURRENT TIME
 * ========================================================= */

struct tm get_current_time(void)
{
    return current_time;
}


/* =========================================================
 * DRAW CLOCK
 * ========================================================= */

void draw_clock(void)
{
    char time_string[16];
    char date_string[32];

    snprintf(
        time_string,
        sizeof(time_string),
        "%02d:%02d:%02d",
        current_time.tm_hour,
        current_time.tm_min,
        current_time.tm_sec
    );

    snprintf(
        date_string,
        sizeof(date_string),
        "%02d/%02d/%04d",
        current_time.tm_mday,
        current_time.tm_mon + 1,
        current_time.tm_year + 1900
    );

    display_show_clock(
        time_string,
        date_string
    );

    last_second = current_time.tm_sec;
}


/* =========================================================
 * UPDATE CLOCK
 * ========================================================= */

void update_clock(void)
{
    time_t now;

    time(&now);

    localtime_r(
        &now,
        &current_time
    );

    /*
     * Only redraw when the displayed second changes.
     */

    if (current_time.tm_sec != last_second) {

        draw_clock();
    }
}