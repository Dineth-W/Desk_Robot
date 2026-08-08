#include "face.h"
#include "display.h"

const int leftEyeX = 90;
const int rightEyeX = 230;
const int eyeY = 90;

const int eyeRadius = 22;

unsigned long previousBlink = 0;
bool blinkState = false;

void drawOpenEyes()
{
    tft.fillCircle(leftEyeX, eyeY, eyeRadius, ILI9341_WHITE);
    tft.fillCircle(rightEyeX, eyeY, eyeRadius, ILI9341_WHITE);

    tft.fillCircle(leftEyeX, eyeY, 8, ILI9341_BLACK);
    tft.fillCircle(rightEyeX, eyeY, 8, ILI9341_BLACK);
}

void drawClosedEyes()
{
    tft.fillRect(leftEyeX-25, eyeY-25, 50, 50, ILI9341_BLACK);
    tft.fillRect(rightEyeX-25, eyeY-25, 50, 50, ILI9341_BLACK);

    tft.drawFastHLine(leftEyeX-22, eyeY, 44, ILI9341_WHITE);
    tft.drawFastHLine(rightEyeX-22, eyeY, 44, ILI9341_WHITE);
}

void drawMouth()
{
    tft.drawCircle(160,170,40,ILI9341_WHITE);
    tft.fillRect(120,130,80,40,ILI9341_BLACK);
}

void faceInit()
{
    tft.fillScreen(ILI9341_BLACK);

    drawOpenEyes();
    drawMouth();
}

void updateFace()
{
    if(millis()-previousBlink>3000)
    {
        previousBlink=millis();

        if(blinkState)
            drawOpenEyes();
        else
            drawClosedEyes();

        blinkState=!blinkState;
    }
}