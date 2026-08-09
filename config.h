#ifndef CONFIG_H
#define CONFIG_H

//==================================================
// TFT ILI9341 (SPI)
//==================================================
#define TFT_CS     5
#define TFT_DC     2
#define TFT_RST    4

#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_SCK    18

//==================================================
// Buttons
//==================================================
#define BTN_ANSWER 13
#define BTN_REJECT 32
#define BTN_MODE   34
#define BTN_WAKE   35

//==================================================
// Buzzer
//==================================================
#define BUZZER_PIN 15

//==================================================
// I2S Microphone (Future)
//==================================================
#define MIC_WS     25
#define MIC_BCLK   26
#define MIC_DATA   33

//==================================================
// I2S Speaker (Future)
//==================================================
#define SPK_DIN    27
#define SPK_BCLK   14
#define SPK_LRC    12

//==================================================
// Robot Timing
//==================================================
#define CLOCK_SCREEN_TIME   10000   // 10 seconds
#define FACE_SCREEN_TIME     5000   // 5 seconds
#define REMINDER_TIME         5000  // popup duration
#define ALARM_TIME           10000

//==================================================
// WiFi
//==================================================
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""

//==================================================
// Time Zone (Sri Lanka)
//==================================================
#define GMT_OFFSET     19800
#define DAYLIGHT_OFFSET 0

#endif
