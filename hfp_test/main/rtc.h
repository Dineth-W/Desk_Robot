#pragma once

#include "esp_err.h"
#include <stdbool.h>
#include <time.h>

esp_err_t ds3231_init(void);
bool ds3231_read(struct tm *t);

esp_err_t ds3231_set_datetime(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second
);