#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

// Initialization
void clockInit();

// Updates
void updateClock();

// Drawing
void drawClock();

// Time Access
bool getCurrentTime(struct tm *timeinfo);
String getTimeString();
String getDateString();

#endif
