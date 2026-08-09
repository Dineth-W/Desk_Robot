#include "display.h"
#include "config.h"

// Create the TFT display object
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

//--------------------------------------------------
// Initialize Display
//--------------------------------------------------
void displayInit()
{
    tft.begin();

    tft.setRotation(1);          // Landscape
    tft.fillScreen(ILI9341_BLACK);
}

//--------------------------------------------------
// Clear Screen
//--------------------------------------------------
void clearScreen()
{
    tft.fillScreen(ILI9341_BLACK);
}

//--------------------------------------------------
// Draw Centered Text
//--------------------------------------------------
void drawCenteredText(String text, int y, uint16_t color, uint8_t size)
{
    tft.setTextColor(color);
    tft.setTextSize(size);

    int16_t x1, y1;
    uint16_t w, h;

    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    int x = (tft.width() - w) / 2;

    tft.setCursor(x, y);
    tft.print(text);
}

//--------------------------------------------------
// Splash Screen
//--------------------------------------------------
void drawSplash()
{
    clearScreen();

    drawCenteredText("Desk Robot", 70, ILI9341_CYAN, 4);

    drawCenteredText("Version 1.0", 130, ILI9341_WHITE, 2);

    drawCenteredText("Starting...", 180, ILI9341_GREEN, 2);
}
