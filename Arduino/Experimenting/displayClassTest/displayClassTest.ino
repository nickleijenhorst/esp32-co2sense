#include "OledDisplay.h"

OledDisplay display;

void setup() {
  Serial.begin(115200);

  display.init();
  
  display.printCO2value(1150);

  delay(3000);

  display.printCO2value(850);
}

void loop() {
}
