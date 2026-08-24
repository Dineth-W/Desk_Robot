#include "rtc.h"
#include "config.h"

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include <stdint.h>

static const char *TAG = "RTC";


/* =========================================================
 * BCD HELPERS
 * ========================================================= */

static uint8_t bcd_to_dec(uint8_t value)
{
    return ((value >> 4) * 10U) +
           (value & 0x0FU);
}


static uint8_t dec_to_bcd(uint8_t value)
{
    return (uint8_t)(
        ((value / 10U) << 4) |
        (value % 10U)
    );
}


/* =========================================================
 * CHECK RTC
 * ========================================================= */

esp_err_t rtc_is_present(void)
{
    uint8_t register_address = 0x00;

    return i2c_master_write_read_device(
        I2C_PORT,
        RTC_ADDRESS,
        &register_address,
        1,
        &register_address,
        1,
        pdMS_TO_TICKS(100)
    );
}


/* =========================================================
 * INITIALIZATION
 * ========================================================= */

esp_err_t rtc_init(void)
{
    esp_err_t err = rtc_is_present();

    if (err != ESP_OK) {

        ESP_LOGE(
            TAG,
            "DS3231 not detected: %s",
            esp_err_to_name(err)
        );

        return err;
    }

    ESP_LOGI(
        TAG,
        "DS3231 detected at 0x%02X",
        RTC_ADDRESS
    );

    return ESP_OK;
}


/* =========================================================
 * READ TIME
 * ========================================================= */

esp_err_t rtc_read(
    struct tm *timeinfo
)
{
    if (timeinfo == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = 0x00;

    uint8_t data[7];

    esp_err_t err =
        i2c_master_write_read_device(
            I2C_PORT,
            RTC_ADDRESS,
            &reg,
            1,
            data,
            sizeof(data),
            pdMS_TO_TICKS(100)
        );

    if (err != ESP_OK) {
        return err;
    }


    /*
     * DS3231:
     *
     * 0 = seconds
     * 1 = minutes
     * 2 = hours
     * 3 = day of week
     * 4 = day
     * 5 = month
     * 6 = year
     */

    uint8_t seconds =
        bcd_to_dec(data[0] & 0x7F);

    uint8_t minutes =
        bcd_to_dec(data[1] & 0x7F);

    uint8_t hours;

    if (data[2] & 0x40) {

        /*
         * 12-hour mode
         */

        hours = bcd_to_dec(
            data[2] & 0x1F
        );

        bool pm =
            (data[2] & 0x20) != 0;

        if (pm && hours < 12) {
            hours += 12;
        }

        if (!pm && hours == 12) {
            hours = 0;
        }

    } else {

        /*
         * 24-hour mode
         */

        hours = bcd_to_dec(
            data[2] & 0x3F
        );
    }


    uint8_t day =
        bcd_to_dec(data[4] & 0x3F);

    uint8_t month =
        bcd_to_dec(data[5] & 0x1F);

    uint8_t year =
        bcd_to_dec(data[6]);


    timeinfo->tm_sec = seconds;
    timeinfo->tm_min = minutes;
    timeinfo->tm_hour = hours;

    timeinfo->tm_mday = day;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_year = 2000 + year - 1900;

    timeinfo->tm_wday =
        (int)(bcd_to_dec(data[3] & 0x07) - 1);

    if (timeinfo->tm_wday < 0) {
        timeinfo->tm_wday = 0;
    }

    timeinfo->tm_isdst = -1;

    return ESP_OK;
}


/* =========================================================
 * WRITE TIME
 * ========================================================= */

esp_err_t rtc_write(
    const struct tm *timeinfo
)
{
    if (timeinfo == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (timeinfo->tm_year < 100) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t data[8];

    data[0] = 0x00;

    data[1] =
        dec_to_bcd(
            (uint8_t)timeinfo->tm_sec
        );

    data[2] =
        dec_to_bcd(
            (uint8_t)timeinfo->tm_min
        );

    data[3] =
        dec_to_bcd(
            (uint8_t)timeinfo->tm_hour
        );

    data[4] =
        dec_to_bcd(
            (uint8_t)(
                timeinfo->tm_wday + 1
            )
        );

    data[5] =
        dec_to_bcd(
            (uint8_t)timeinfo->tm_mday
        );

    data[6] =
        dec_to_bcd(
            (uint8_t)(
                timeinfo->tm_mon + 1
            )
        );

    data[7] =
        dec_to_bcd(
            (uint8_t)(
                timeinfo->tm_year - 100
            )
        );


    return i2c_master_write_to_device(
        I2C_PORT,
        RTC_ADDRESS,
        data,
        sizeof(data),
        pdMS_TO_TICKS(100)
    );
}