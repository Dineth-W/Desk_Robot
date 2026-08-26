#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;

// Initialization
void displayInit();

// Common screen functions
void clearScreen();
void drawSplash();

// Utility functions
void drawCenteredText(String text, int y, uint16_t color, uint8_t size);

#endif
