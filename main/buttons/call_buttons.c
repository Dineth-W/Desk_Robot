#include "call_buttons.h"

#include "driver/gpio.h"

#define ANSWER_BUTTON_GPIO GPIO_NUM_32
#define END_BUTTON_GPIO    GPIO_NUM_34

static bool s_answer_previous = false;
static bool s_end_previous = false;


void call_buttons_init(void)
{
    /*
     * ANSWER BUTTON
     *
     * GPIO32
     *
     * GPIO32 ---- button ---- GND
     *
     * Internal pull-up enabled.
     */

    gpio_config_t answer_config = {
        .pin_bit_mask = 1ULL << ANSWER_BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&answer_config);


    /*
     * END BUTTON
     *
     * GPIO34 does NOT have an internal pull-up.
     *
     * Hardware:
     *
     *        3.3V
     *         |
     *        10k
     *         |
     * GPIO34 --+---- button ---- GND
     */

    gpio_config_t end_config = {
        .pin_bit_mask = 1ULL << END_BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&end_config);
}


bool call_button_answer_pressed(void)
{
    bool pressed =
        gpio_get_level(ANSWER_BUTTON_GPIO) == 0;

    bool event =
        pressed && !s_answer_previous;

    s_answer_previous = pressed;

    return event;
}


bool call_button_end_pressed(void)
{
    bool pressed =
        gpio_get_level(END_BUTTON_GPIO) == 0;

    bool event =
        pressed && !s_end_previous;

    s_end_previous = pressed;

    return event;
}