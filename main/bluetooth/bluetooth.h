#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_err.h"

esp_err_t bluetooth_init(void);
esp_err_t bluetooth_answer(void);
esp_err_t bluetooth_reject(void);
esp_err_t bluetooth_hangup(void);

#endif // BLUETOOTH_H