#include <Credentials.h>

Credentials credentials;

void setup() {
  Serial.begin(115200);

  credentials.init();

  Serial.println("Loading credentials...");

  credentials.load();

  credentials.print();

  credentials.ssid.set("My SSID1");
  credentials.password.set("My SSID2");
  credentials.deviceid.set("My SSID3");
  credentials.username.set("My SSID4");

  Serial.println("Changed credentials, printing again:");
  credentials.print();

  credentials.save();
  }

void loop() {
  // put your main code here, to run repeatedly:

}
