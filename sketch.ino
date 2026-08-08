#include "display.h"
#include "clock.h"
#include "face.h"
#include "todo.h"

unsigned long screenTimer = 0;

bool showClock = true;

void setup()
{
    displayInit();

    splashScreen();

    delay(2000);

    clockInit();

    todoInit(); 

    screenTimer = millis();
}

void loop()
{

    updateTodo();
    if(showClock)
    {
        updateClock();

        if(millis()-screenTimer>10000)
        {
            showClock=false;

            screenTimer=millis();

            faceInit();
        }
    }
    else
    {
        updateFace();

        if(millis()-screenTimer>5000)
        {
            showClock=true;

            screenTimer=millis();
        }
    }

    delay(50);
}