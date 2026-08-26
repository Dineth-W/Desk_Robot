#include "face.h"
#include "display.h"

// Internal Variables
static FaceExpression currentExpression = FACE_NORMAL;

static unsigned long animationTimer = 0;
static unsigned long blinkTimer = 0;

static bool eyesOpen = true;
static int eyeOffset = 0;
static bool mouthOpen = false;

// Face Dimensions
const int LEFT_EYE_X = 95;
const int RIGHT_EYE_X = 225;
const int EYE_Y = 90;

const int EYE_RADIUS = 20;

const int MOUTH_X = 160;
const int MOUTH_Y = 185;

// Draw Eyes
void drawEyes()
{
    // Erase previous eye area
    tft.fillRect(40, 40, 240, 90, ILI9341_BLACK);

    if (eyesOpen)
    {
        // White eyes
        tft.fillCircle(LEFT_EYE_X, EYE_Y, EYE_RADIUS, ILI9341_WHITE);
        tft.fillCircle(RIGHT_EYE_X, EYE_Y, EYE_RADIUS, ILI9341_WHITE);

        // Pupils
        tft.fillCircle(LEFT_EYE_X + eyeOffset, EYE_Y, 8, ILI9341_BLACK);
        tft.fillCircle(RIGHT_EYE_X + eyeOffset, EYE_Y, 8, ILI9341_BLACK);
    }
    else
    {
        tft.drawFastHLine(LEFT_EYE_X - 20, EYE_Y, 40, ILI9341_WHITE);
        tft.drawFastHLine(RIGHT_EYE_X - 20, EYE_Y, 40, ILI9341_WHITE);
    }
}


// Draw Mouth
void drawMouth()
{
    tft.fillRect(100, 150, 120, 80, ILI9341_BLACK);

    switch (currentExpression)
    {
        case FACE_NORMAL:

            tft.drawCircle(MOUTH_X, MOUTH_Y, 30, ILI9341_WHITE);
            tft.fillRect(130, 150, 60, 30, ILI9341_BLACK);

            break;

        case FACE_HAPPY:

            tft.drawCircle(MOUTH_X, MOUTH_Y - 10, 35, ILI9341_GREEN);
            tft.fillRect(120, 110, 80, 35, ILI9341_BLACK);

            break;

        case FACE_SAD:

            tft.drawCircle(MOUTH_X, MOUTH_Y + 25, 35, ILI9341_RED);
            tft.fillRect(120, 210, 80, 40, ILI9341_BLACK);

            break;

        case FACE_SLEEP:

            tft.drawFastHLine(130, 185, 60, ILI9341_BLUE);

            break;

        case FACE_SURPRISED:

            tft.drawCircle(MOUTH_X, MOUTH_Y, 18, ILI9341_YELLOW);

            break;

        case FACE_TALKING:

            if (mouthOpen)
            {
                tft.fillRoundRect(145, 165, 30, 40, 8, ILI9341_WHITE);
            }
            else
            {
                tft.drawFastHLine(140, 185, 40, ILI9341_WHITE);
            }

            break;
    }
}

// Draw Entire Face
void drawFace()
{
    clearScreen();

    drawEyes();
    drawMouth();
}


// Initialize Face
void faceInit()
{
    currentExpression = FACE_NORMAL;

    eyesOpen = true;
    eyeOffset = 0;

    drawFace();

    animationTimer = millis();
    blinkTimer = millis();
}


// Update Animation
void updateFace()
{
    // Blink every 4 seconds
    if (millis() - blinkTimer > 4000)
    {
        eyesOpen = !eyesOpen;

        drawEyes();

        if (!eyesOpen)
        {
            delay(120);

            eyesOpen = true;

            drawEyes();
        }

        blinkTimer = millis();
    }

    // Eye movement every second
    if (millis() - animationTimer > 1000)
    {
        animationTimer = millis();

        static int state = 0;

        switch (state)
        {
            case 0:
                eyeOffset = -5;
                break;

            case 1:
                eyeOffset = 0;
                break;

            case 2:
                eyeOffset = 5;
                break;

            case 3:
                eyeOffset = 0;
                break;
        }

        state++;

        if (state > 3)
            state = 0;

        if (currentExpression == FACE_TALKING)
        {
            mouthOpen = !mouthOpen;
        }

        drawEyes();
        drawMouth();
    }
}

// Change Expression
void setExpression(FaceExpression expression)
{
    currentExpression = expression;

    drawFace();
}

// Get Current Expression
FaceExpression getExpression()
{
    return currentExpression;
}
