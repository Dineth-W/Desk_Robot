#include "clock.h"
#include "display.h"

#include <WiFi.h>
#include <time.h>

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP
const char* ntpServer = "pool.ntp.org";

// Sri Lanka
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

// Previous values
int prevHour = -1;
int prevMinute = -1;
int prevSecond = -1;

// Character positions
const int xHour = 35;
const int xColon1 = 115;
const int xMinute = 135;
const int xColon2 = 215;
const int xSecond = 235;

const int yTime = 100;

//--------------------------------------------------

void clockInit()
{
    WiFi.begin(ssid, password);

    tft.fillScreen(ILI9341_BLACK);

    tft.setCursor(20,20);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("Connecting WiFi...");

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
    }

    configTime(
        gmtOffset_sec,
        daylightOffset_sec,
        ntpServer);

    delay(500);

    drawClock();
}

//--------------------------------------------------

void drawTwoDigits(int value,int x)
{
    char txt[3];

    sprintf(txt,"%02d",value);

    tft.setTextColor(
        ILI9341_CYAN,
        ILI9341_BLACK);

    tft.setTextSize(5);

    tft.setCursor(x,yTime);

    tft.print(txt);
}

//--------------------------------------------------

void drawClock()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextSize(5);
    tft.setTextColor(ILI9341_CYAN);

    tft.setCursor(xColon1,yTime);
    tft.print(":");

    tft.setCursor(xColon2,yTime);
    tft.print(":");

    prevHour = -1;
    prevMinute = -1;
    prevSecond = -1;
}

//--------------------------------------------------

void updateClock()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
        return;

    if(timeinfo.tm_hour != prevHour)
    {
        drawTwoDigits(
            timeinfo.tm_hour,
            xHour);

        prevHour = timeinfo.tm_hour;
    }

    if(timeinfo.tm_min != prevMinute)
    {
        drawTwoDigits(
            timeinfo.tm_min,
            xMinute);

        prevMinute = timeinfo.tm_min;
    }

    if(timeinfo.tm_sec != prevSecond)
    {
        drawTwoDigits(
            timeinfo.tm_sec,
            xSecond);

        prevSecond = timeinfo.tm_sec;
    }
}
