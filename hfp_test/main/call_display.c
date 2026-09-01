#include "call_display.h"

#include "display.h"
#include "esp_timer.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = "CALL_DISPLAY";
static int64_t s_ended_time = 0;

typedef enum {
    CALL_UI_IDLE,
    CALL_UI_INCOMING,
    CALL_UI_ACTIVE,
    CALL_UI_ENDED
} call_ui_state_t;

static call_ui_state_t s_state = CALL_UI_IDLE;

static char s_number[24] = "Unknown";

static int64_t s_start_time = 0;
static int64_t s_last_animation = 0;

static int s_animation_frame = 0;


/* =========================================================
 * Small phone/ringing icon
 * ========================================================= */

static void draw_phone_icon(int x, int y, bool ringing)
{
    /*
     * Simple handset shape.
     *
     * We deliberately draw this ourselves rather than
     * using a font/icon library.
     */

    display_draw_hline(x - 7, y - 7, 5, true);
    display_draw_hline(x + 2, y + 2, 5, true);

    display_draw_vline(x - 7, y - 7, 6, true);
    display_draw_vline(x + 6, y + 2, 6, true);

    display_draw_hline(x - 7, y - 2, 4, true);
    display_draw_hline(x + 3, y + 6, 4, true);

    if (ringing) {

        /*
         * Ring waves.
         */
        display_draw_hline(
            x - 13,
            y - 10,
            3,
            true
        );

        display_draw_hline(
            x + 10,
            y - 10,
            3,
            true
        );

        display_draw_hline(
            x - 16,
            y - 6,
            3,
            true
        );

        display_draw_hline(
            x + 13,
            y - 6,
            3,
            true
        );
    }
}


/* =========================================================
 * Incoming call screen
 * ========================================================= */

static void draw_incoming(void)
{
    display_clear();

    /*
     * Header
     */
    display_center_text(
        "INCOMING",
        2,
        1,
        true
    );

    /*
     * Animated phone.
     */
    bool ringing =
        (s_animation_frame % 2) == 0;

    draw_phone_icon(
        64,
        29,
        ringing
    );

    /*
     * Caller number.
     */
    display_center_text(
        s_number,
        45,
        1,
        true
    );

    /*
     * Bottom controls.
     */
    display_text(
        3,
        57,
        "ANS",
        1,
        true
    );

    display_text(
        101,
        57,
        "END",
        1,
        true
    );

    display_update();
}


/* =========================================================
 * Active call screen
 * ========================================================= */

static void draw_active(void)
{
    display_clear();

    display_center_text(
        "CALL ACTIVE",
        2,
        1,
        true
    );

    /*
     * Calculate elapsed call time.
     */
    int64_t now =
        esp_timer_get_time() / 1000;

    uint32_t elapsed =
        (uint32_t)((now - s_start_time) / 1000);

    uint32_t minutes =
        elapsed / 60;

    uint32_t seconds =
        elapsed % 60;

    char timer[16];

    snprintf(
        timer,
        sizeof(timer),
        "%02lu:%02lu",
        (unsigned long)minutes,
        (unsigned long)seconds
    );

    /*
     * Large timer.
     */
    display_center_text(
        timer,
        20,
        2,
        true
    );

    /*
     * Caller number.
     */
    display_center_text(
        s_number,
        43,
        1,
        true
    );

    /*
     * END button.
     */
    display_center_text(
        "END CALL",
        56,
        1,
        true
    );

    display_update();
}


/* =========================================================
 * Ended screen
 * ========================================================= */

static void draw_ended(void)
{
    display_clear();

    display_center_text(
        "CALL ENDED",
        20,
        1,
        true
    );

    display_center_text(
        "GOODBYE",
        38,
        1,
        true
    );

    display_update();
}


/* =========================================================
 * Initialization
 * ========================================================= */

void call_display_init(void)
{
    s_state = CALL_UI_IDLE;

    s_number[0] = '\0';

    s_start_time = 0;
    s_last_animation = 0;
    s_animation_frame = 0;

    ESP_LOGI(
        TAG,
        "Call display initialized"
    );
}


/* =========================================================
 * Idle
 * ========================================================= */

void call_display_idle(void)
{
    s_state = CALL_UI_IDLE;

    ESP_LOGI(
        TAG,
        "[CALL UI] IDLE"
    );
}


/* =========================================================
 * Incoming call
 * ========================================================= */

void call_display_incoming(
    const char *number
)
{
    if (number && number[0]) {

        strncpy(
            s_number,
            number,
            sizeof(s_number) - 1
        );

        s_number[
            sizeof(s_number) - 1
        ] = '\0';

    } else {

        strcpy(
            s_number,
            "Unknown"
        );
    }

    s_state = CALL_UI_INCOMING;

    s_animation_frame = 0;

    s_last_animation =
        esp_timer_get_time() / 1000;

    draw_incoming();

    ESP_LOGI(
        TAG,
        "[CALL UI] INCOMING CALL: %s",
        s_number
    );
}


/* =========================================================
 * Active call
 * ========================================================= */

void call_display_active(
    const char *number
)
{
    if (number && number[0]) {

        strncpy(
            s_number,
            number,
            sizeof(s_number) - 1
        );

        s_number[
            sizeof(s_number) - 1
        ] = '\0';

    }

    s_state = CALL_UI_ACTIVE;

    s_start_time =
        esp_timer_get_time() / 1000;

    s_last_animation =
        s_start_time;

    draw_active();

    ESP_LOGI(
        TAG,
        "[CALL UI] CALL ACTIVE"
    );
}


/* =========================================================
 * Call ended
 * ========================================================= */

void call_display_ended(void)
{
    s_state = CALL_UI_ENDED;

    s_ended_time =
        esp_timer_get_time() / 1000;

    draw_ended();

    ESP_LOGI(
        TAG,
        "[CALL UI] CALL ENDED"
    );
}


/* =========================================================
 * Animation update
 * ========================================================= */

void call_display_update(void)
{
    int64_t now =
        esp_timer_get_time() / 1000;

    /* ================================
     * INCOMING CALL
     * ================================ */

    if (s_state == CALL_UI_INCOMING) {

        if (now - s_last_animation >= 350) {

            s_animation_frame++;

            s_last_animation = now;

            draw_incoming();
        }
    }

    /* ================================
     * ACTIVE CALL
     * ================================ */

    else if (s_state == CALL_UI_ACTIVE) {

        if (now - s_last_animation >= 1000) {

            s_last_animation = now;

            draw_active();
        }
    }

    /* ================================
     * CALL ENDED
     * ================================ */

    else if (s_state == CALL_UI_ENDED) {

        if (now - s_ended_time >= 1500) {

            /*
             * Release the display back to the
             * normal clock/face scheduler.
             */
            s_state = CALL_UI_IDLE;

            ESP_LOGI(
                TAG,
                "[CALL UI] CALL UI RELEASED"
            );
        }
    }
}


/* =========================================================
 * Display ownership
 * ========================================================= */

bool call_display_is_active(void)
{
    return s_state != CALL_UI_IDLE;
}