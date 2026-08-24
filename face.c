#include "face.h"
#include "display.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdint.h>

static const char *TAG = "FACE";


static FaceExpression current_expression =
    FACE_NORMAL;

static uint32_t last_animation = 0;
static uint32_t last_blink = 0;

static int eyes_open = 1;
static int mouth_open = 0;


/* =========================================================
 * DRAW FACE
 * ========================================================= */

void draw_face(void)
{
    display_clear();

    switch (current_expression) {

        case FACE_HAPPY:

            display_center_text(
                "^   ^",
                12,
                2
            );

            display_center_text(
                "\\___/",
                38,
                1
            );

            break;


        case FACE_SAD:

            display_center_text(
                "T   T",
                12,
                2
            );

            display_center_text(
                "/---\\",
                38,
                1
            );

            break;


        case FACE_SLEEP:

            display_center_text(
                "-   -",
                12,
                2
            );

            display_center_text(
                "zzz",
                40,
                1
            );

            break;


        case FACE_SURPRISED:

            display_center_text(
                "O   O",
                12,
                2
            );

            display_center_text(
                "  O  ",
                40,
                1
            );

            break;


        case FACE_TALKING:

            display_center_text(
                "O   O",
                12,
                2
            );

            if (mouth_open) {

                display_center_text(
                    "  O  ",
                    40,
                    1
                );

            } else {

                display_center_text(
                    " --- ",
                    40,
                    1
                );
            }

            break;


        case FACE_NORMAL:

        default:

            if (eyes_open) {

                display_center_text(
                    "O   O",
                    12,
                    2
                );

            } else {

                display_center_text(
                    "-   -",
                    12,
                    2
                );
            }

            display_center_text(
                "-----",
                40,
                1
            );

            break;
    }

    display_refresh();
}


/* =========================================================
 * INITIALIZATION
 * ========================================================= */

void face_init(void)
{
    current_expression = FACE_NORMAL;

    eyes_open = 1;
    mouth_open = 0;

    last_animation = 0;
    last_blink = 0;

    draw_face();

    ESP_LOGI(
        TAG,
        "Face initialized."
    );
}


/* =========================================================
 * UPDATE
 * ========================================================= */

void update_face(void)
{
    uint32_t now =
        (uint32_t)(
            xTaskGetTickCount() *
            portTICK_PERIOD_MS
        );


    /* -----------------------------------------------------
     * BLINK
     * ----------------------------------------------------- */

    if (eyes_open &&
        (now - last_blink >= 3500U)) {

        eyes_open = 0;
        last_blink = now;

        draw_face();

        return;
    }


    if (!eyes_open &&
        (now - last_blink >= 120U)) {

        eyes_open = 1;
        last_blink = now;

        draw_face();

        return;
    }


    /* -----------------------------------------------------
     * TALKING ANIMATION
     * ----------------------------------------------------- */

    if (current_expression == FACE_TALKING &&
        (now - last_animation >= 700U)) {

        last_animation = now;

        mouth_open = !mouth_open;

        draw_face();
    }
}


/* =========================================================
 * SET EXPRESSION
 * ========================================================= */

void set_expression(
    FaceExpression expression
)
{
    current_expression = expression;

    draw_face();
}


/* =========================================================
 * GET EXPRESSION
 * ========================================================= */

FaceExpression get_expression(void)
{
    return current_expression;
}