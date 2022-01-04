#include <OledDisplay.h>

OledDisplay display;

void setup() {
  Serial.begin(115200);

  display.init();

  delay(1000);
  
  display.printSplashScreen();
  delay(2000);
  display.writeTextToScreen("Hehe this works", 1);
}

void loop() {
}
