#ifndef CONFIG_H
#define CONFIG_H

#include "driver/i2c.h"

/* =========================================================
 * OLED / I2C
 * ========================================================= */

#define OLED_SDA        21
#define OLED_SCL        22

#define OLED_WIDTH      128
#define OLED_HEIGHT     64

#define OLED_ADDRESS    0x3C
#define RTC_ADDRESS     0x68

#define I2C_PORT        I2C_NUM_0
#define I2C_FREQUENCY   100000


/* =========================================================
 * BUTTONS
 * ========================================================= */

#define BTN_1           32

/*
 * GPIO32 -> button -> GND
 *
 * Internal pull-up is used.
 */


/* =========================================================
 * AUDIO
 * ========================================================= */

#define I2S_BCLK        25
#define I2S_WS          26

#define MIC_DATA        33
#define SPEAKER_DATA    27


/* =========================================================
 * BUZZER
 * ========================================================= */

#define BUZZER_PIN      26
#define BUZZER_FREQ     2000


/* =========================================================
 * DISPLAY TIMING
 * ========================================================= */

#define CLOCK_SCREEN_TIME       10000UL
#define FACE_SCREEN_TIME         5000UL
#define REMINDER_SCREEN_TIME     5000UL


/* =========================================================
 * ALARMS
 * ========================================================= */

#define MAX_ALARMS 10


/* =========================================================
 * CALL SIMULATION
 * ========================================================= */

#define CALL_RING_INTERVAL       600UL
#define CALL_SIMULATION_INTERVAL 60000UL


#endif