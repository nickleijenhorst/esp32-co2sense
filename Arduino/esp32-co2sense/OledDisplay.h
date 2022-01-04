#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C

class OledDisplay {

public:
    OledDisplay(void);
    void init();
    void printSplashScreen();
    void clearDisplay();
    void writeText(const char *text, uint8_t textSize);
    void connectingToWifi();
    void printDot();
    void successConnectedToWifi();
    
    Adafruit_SSD1306* display;
};

#endif
