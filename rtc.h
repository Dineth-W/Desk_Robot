#ifndef RTC_H
#define RTC_H

#include "esp_err.h"

#include <time.h>

esp_err_t rtc_init(void);

esp_err_t rtc_read(struct tm *timeinfo);

esp_err_t rtc_write(
    const struct tm *timeinfo
);

esp_err_t rtc_is_present(void);

#endif