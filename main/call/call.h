#ifndef CALL_H
#define CALL_H

#include "esp_err.h"

typedef enum {
    CALL_IDLE,
    CALL_INCOMING,
    CALL_ACTIVE
} call_state_t;

esp_err_t call_init(void);
call_state_t call_get_state(void);
esp_err_t call_answer(void);
esp_err_t call_reject(void);
esp_err_t call_end(void);

#endif // CALL_H