#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "config.h"
#include "display.h"

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

void displayInit()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
}

void splashScreen()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(3);

    tft.setCursor(45,60);
    tft.println("Desk Robot");

    tft.setTextSize(2);

    tft.setCursor(65,120);
    tft.println("Starting...");
}