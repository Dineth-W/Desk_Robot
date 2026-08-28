#include "rtc.h"
#include "config.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG="RTC";
static i2c_master_dev_handle_t dev;

static uint8_t bcd2dec(uint8_t x){ return (x>>4)*10 + (x&0x0F); }

esp_err_t ds3231_init(void)
{
    if (i2c_probe(DS3231_I2C_ADDR) != ESP_OK) {
        ESP_LOGW(TAG,"DS3231 not detected at 0x68");
        return ESP_ERR_NOT_FOUND;
    }

    i2c_device_config_t cfg={
        .dev_addr_length=I2C_ADDR_BIT_LEN_7,
        .device_address=DS3231_I2C_ADDR,
        .scl_speed_hz=100000,
    };
    esp_err_t e=i2c_master_bus_add_device(i2c_bus_get_handle(),&cfg,&dev);
    if(e==ESP_OK) ESP_LOGI(TAG,"DS3231 detected");
    return e;
}

bool ds3231_read(struct tm *t)
{
    if(!dev || !t) return false;
    uint8_t reg=0, d[7];
    if(i2c_master_transmit_receive(dev,&reg,1,d,7,100)!=ESP_OK) return false;

    memset(t,0,sizeof(*t));
    t->tm_sec=bcd2dec(d[0]&0x7F);
    t->tm_min=bcd2dec(d[1]&0x7F);
    t->tm_hour=bcd2dec(d[2]&0x3F);
    t->tm_mday=bcd2dec(d[4]&0x3F);
    t->tm_mon=bcd2dec(d[5]&0x1F)-1;
    t->tm_year=2000+bcd2dec(d[6])-1900;
    return true;
}

static uint8_t dec2bcd(uint8_t x)
{
    return ((x / 10) << 4) | (x % 10);
}

esp_err_t ds3231_set_datetime(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second)
{
    if (!dev) {
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t data[8];

    data[0] = 0x00;                 // Start register
    data[1] = dec2bcd(second);
    data[2] = dec2bcd(minute);
    data[3] = dec2bcd(hour);
    data[4] = 1;                    // Day of week, not important
    data[5] = dec2bcd(day);
    data[6] = dec2bcd(month);
    data[7] = dec2bcd(year - 2000);

    return i2c_master_transmit(
        dev,
        data,
        sizeof(data),
        100
    );
}