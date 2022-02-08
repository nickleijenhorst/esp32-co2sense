#include "OledDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>

OledDisplay::OledDisplay() {
  display = new Adafruit_SSD1306(128, 64, &Wire, OLED_RESET);
}

void OledDisplay::init() {
  
    if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  clearDisplay();
}

void OledDisplay::printSplashScreen() {
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(18, 10);
    display->cp437(true);
    display->print("CO2Sense");

    display->setCursor(0, 45);
    display->setTextSize(1);
    display->print("Developed by:");
    display->setCursor(0, 55);
    display->print("Nick Leijenhorst");
    display->display();
}

void OledDisplay::clearDisplay() {
  display->clearDisplay();
  display->display();
}

void OledDisplay::writeText(const char *text, uint8_t textSize) {
    display->clearDisplay();
    display->setTextSize(textSize);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->cp437(true);
    display->write(text);
    display->display();
}

void OledDisplay::connectingToWifi() {
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 20);
    display->cp437(true);
    display->write("Connecting to WiFi...\n\n");
    display->display();
}

void OledDisplay::printDot() {
    display->write(".");
    display->display();
}

void OledDisplay::successConnectedToWifi() {
    display->clearDisplay();
    display->setTextSize(1);
    display->setCursor(5, 25);
    display->cp437(true);
    display->write("Connected to Wi-Fi!");
    display->display();
}

void OledDisplay::printCO2value(int value) {

    //Limit values
    if(value < 100){value = 100;}
    if(value > 9999){value = 9999;}
    
    char str[5];
    sprintf(str, "%d", value);
  
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->cp437(true);
    display->write("CO2");

    //18px wide
    display->setTextSize(3);

    int co2TextWidth = strlen(str) * 18;
    int ppmTextWidth = 18;
    
    display->setCursor(64 - ((co2TextWidth+ppmTextWidth)/2), 30);
    display->write(str);

    display->setTextSize(1);
    display->setCursor(64 + ((co2TextWidth)/2) - 4, 30);
    display->write("PPM");
    display->display();
}
