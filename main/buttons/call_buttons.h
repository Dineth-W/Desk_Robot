#ifndef CALL_BUTTONS_H
#define CALL_BUTTONS_H

#include "driver/gpio.h"

#define CALL_ANSWER_BUTTON_GPIO    GPIO_NUM_32
#define CALL_END_BUTTON_GPIO       GPIO_NUM_13

void call_buttons_init(void);

#endif // CALL_BUTTONS_H