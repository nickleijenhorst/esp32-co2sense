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

void OledDisplay::writeTextToScreen(const char *text, uint8_t textSize) {
    display->clearDisplay();
    display->setTextSize(textSize);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->cp437(true);
    display->write(text);
    display->display();
}
