#include "face.h"
#include "display.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static face_expression_t expr = FACE_NORMAL;

static bool blink = false;
static int eye_offset_x = 0;
static int eye_offset_y = 0;

static int mouth_state = 0;

static int64_t last_blink = 0;
static int64_t last_eye_move = 0;
static int64_t last_mouth_move = 0;

static int blink_duration_ms = 130;

static void draw_eye(int x, int y)
{
    if (blink) {
        /*
         * Closed eye
         */
        display_draw_hline(
            x - 7,
            y,
            14,
            true
        );
        return;
    }

    /*
     * Eye outline / eyeball
     */
    display_fill_rect(
        x - 7,
        y - 7,
        15,
        15,
        true
    );

    /*
     * Eye pupil / inner area
     */
    display_fill_rect(
        x - 3 + eye_offset_x,
        y - 3 + eye_offset_y,
        7,
        7,
        false
    );
}

static void draw_mouth(void)
{
    if (expr == FACE_HAPPY) {

        /*
         * Happy smile
         */
        display_draw_hline(54, 48, 20, true);
        display_draw_hline(56, 50, 16, true);

    } else if (expr == FACE_SAD) {

        /*
         * Sad mouth
         */
        display_draw_hline(54, 50, 20, true);
        display_draw_hline(56, 48, 16, true);

    } else if (expr == FACE_SURPRISED) {

        /*
         * Open surprised mouth
         */
        display_draw_rect(
            58,
            43,
            12,
            12,
            true
        );

    } else if (expr == FACE_SLEEP) {

        display_draw_hline(
            55,
            49,
            18,
            true
        );

    } else if (expr == FACE_TALKING) {

        /*
         * Talking animation
         */
        if (mouth_state == 0) {

            display_draw_hline(
                55,
                49,
                18,
                true
            );

        } else if (mouth_state == 1) {

            display_fill_rect(
                57,
                45,
                14,
                8,
                true
            );

        } else {

            display_draw_rect(
                58,
                44,
                12,
                10,
                true
            );
        }

    } else {

        /*
         * Normal mouth
         */
        display_draw_hline(
            55,
            49,
            18,
            true
        );
    }
}

void face_draw(void)
{
    display_clear();

    /*
     * Left eye
     */
    draw_eye(
        38,
        27
    );

    /*
     * Right eye
     */
    draw_eye(
        90,
        27
    );

    /*
     * Mouth
     */
    draw_mouth();

    display_update();
}

void face_init(void)
{
    expr = FACE_HAPPY;

    blink = false;

    eye_offset_x = 0;
    eye_offset_y = 0;

    mouth_state = 0;

    last_blink = esp_timer_get_time() / 1000;
    last_eye_move = last_blink;
    last_mouth_move = last_blink;

    face_draw();
}

void face_set(face_expression_t e)
{
    expr = e;

    blink = false;

    eye_offset_x = 0;
    eye_offset_y = 0;

    mouth_state = 0;

    face_draw();
}

void face_update(void)
{
    int64_t now =
        esp_timer_get_time() / 1000;

    /*
     * =========================================================
     * BLINK
     * =========================================================
     *
     * Blink approximately every 1.5–2.5 seconds.
     */
    if (!blink &&
        now - last_blink > 1800) {

        blink = true;

        face_draw();

        vTaskDelay(
            pdMS_TO_TICKS(blink_duration_ms)
        );

        blink = false;

        last_blink =
            esp_timer_get_time() / 1000;

        face_draw();
    }

    /*
     * =========================================================
     * EYE MOVEMENT
     * =========================================================
     *
     * Move eyes every ~600 ms.
     */
    if (now - last_eye_move > 600) {

        static int direction = 0;

        direction++;

        switch (direction % 7) {

            case 0:
                /* Center */
                eye_offset_x = 0;
                eye_offset_y = 0;
                break;

            case 1:
                /* Look left */
                eye_offset_x = -2;
                eye_offset_y = 0;
                break;

            case 2:
                /* Center */
                eye_offset_x = 0;
                eye_offset_y = 0;
                break;

            case 3:
                /* Look right */
                eye_offset_x = 2;
                eye_offset_y = 0;
                break;

            case 4:
                /* Look upper right */
                eye_offset_x = 2;
                eye_offset_y = -2;
                break;

            case 5:
                /* Look lower left */
                eye_offset_x = -2;
                eye_offset_y = 2;
                break;

            case 6:
                /* Center */
                eye_offset_x = 0;
                eye_offset_y = 0;
                break;
        }

        last_eye_move = now;

        face_draw();
    }

    /*
     * =========================================================
     * MOUTH ANIMATION
     * =========================================================
     */
    if (expr == FACE_TALKING &&
        now - last_mouth_move > 250) {

        mouth_state++;

        if (mouth_state > 2) {
            mouth_state = 0;
        }

        last_mouth_move = now;

        face_draw();
    }
}