#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"

#define OLED_WIDTH              128
#define OLED_HEIGHT             64
#define OLED_I2C_ADDR           0x3C

#define DS3231_I2C_ADDR         0x68

#define I2C_PORT                I2C_NUM_0
#define I2C_SDA_GPIO            GPIO_NUM_21
#define I2C_SCL_GPIO            GPIO_NUM_22

#define BTN_ANSWER_GPIO         GPIO_NUM_32
#define BTN_REJECT_GPIO         GPIO_NUM_34

#define BUZZER_GPIO             GPIO_NUM_26
#define BUZZER_FREQ_HZ          2000
#define BUZZER_DUTY             128

#define I2S_BCLK_GPIO           GPIO_NUM_25
#define I2S_WS_GPIO             GPIO_NUM_26
#define I2S_MIC_DATA_GPIO       GPIO_NUM_33
#define I2S_SPK_DATA_GPIO       GPIO_NUM_27

#define AUDIO_SAMPLE_RATE       16000
#define AUDIO_BITS              32
#define AUDIO_BUFFER_SAMPLES    256

#define CALL_RING_PERIOD_MS     600
#define REMINDER_DURATION_MS    5000
#define CLOCK_FACE_CYCLE_MS     10000
#define FACE_CLOCK_CYCLE_MS     5000
