#include "buttons.h"
#include "config.h"
#include "driver/gpio.h"

static bool last_a=true,last_r=true;

void buttons_init(void)
{
    gpio_config_t a={.pin_bit_mask=1ULL<<BTN_ANSWER_GPIO,
        .mode=GPIO_MODE_INPUT,.pull_up_en=GPIO_PULLUP_ENABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,.intr_type=GPIO_INTR_DISABLE};
    gpio_config(&a);

    gpio_config_t r={.pin_bit_mask=1ULL<<BTN_REJECT_GPIO,
        .mode=GPIO_MODE_INPUT,.pull_up_en=GPIO_PULLUP_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,.intr_type=GPIO_INTR_DISABLE};
    gpio_config(&r);
}

bool button_answer_pressed(void)
{
    bool now=gpio_get_level(BTN_ANSWER_GPIO)==0;
    bool event=now && last_a;
    last_a=!now;
    return event;
}

bool button_reject_pressed(void)
{
    bool now=gpio_get_level(BTN_REJECT_GPIO)==0;
    bool event=now && last_r;
    last_r=!now;
    return event;
}
