#ifndef CONFIG_H
#define CONFIG_H

//======================
// TFT Display
//======================
#define TFT_CS      5
#define TFT_DC      2
#define TFT_RST     4

//======================
// SPI
//======================
#define TFT_MOSI    23
#define TFT_MISO    19
#define TFT_SCK     18

//======================
// Buttons
//======================
#define BTN_ANSWER  13
#define BTN_REJECT  32
#define BTN_MODE    34
#define BTN_WAKE    35

//======================
// Buzzer
//======================
#define BUZZER_PIN  15

//======================
// Microphone
//======================
#define MIC_WS      25
#define MIC_SCK     26
#define MIC_SD      33

//======================
// Speaker
//======================
#define SPK_DIN     27
#define SPK_BCLK    14
#define SPK_LRC     12

#endif