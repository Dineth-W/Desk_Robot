#pragma once
#include "esp_err.h"
#include <time.h>

esp_err_t rtc_init(void);
bool rtc_read(struct tm *t);
