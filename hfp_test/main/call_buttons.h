#ifndef CALL_BUTTONS_H
#define CALL_BUTTONS_H

#include <stdbool.h>
#include "esp_err.h"

/*
 * Desk Robot call buttons
 *
 * ANSWER:
 * GPIO32
 *
 * END / REJECT:
 * GPIO34
 *
 * Buttons are active LOW.
 */

#define CALL_BUTTON_ANSWER_GPIO   32
#define CALL_BUTTON_END_GPIO      13

esp_err_t call_buttons_init(void);

/*
 * Update the current HFP call state.
 *
 * incoming = true  -> incoming call is ringing
 * active   = true  -> call is currently active
 */
void call_buttons_set_state(bool incoming, bool active);

#endif /* CALL_BUTTONS_H */