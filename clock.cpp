#include "clock.h"
#include "display.h"

#include <WiFi.h>
#include <time.h>

// WiFi (Wokwi)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP
const char* ntpServer = "pool.ntp.org";

// Sri Lanka (UTC +5:30)
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

void clockInit()
{
    WiFi.begin(ssid, password);

    tft.fillScreen(ILI9341_BLACK);

    tft.setCursor(20,20);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("Connecting WiFi...");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    configTime(gmtOffset_sec,
               daylightOffset_sec,
               ntpServer);

    delay(500);
}

void updateClock()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
        return;

    char buffer[20];

    strftime(buffer,sizeof(buffer),"%H:%M:%S",&timeinfo);

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(5);

    tft.setCursor(25,100);
    tft.println(buffer);
}