#include <Credentials.h>

Credentials credentials;

void setup() {
  Serial.begin(115200);

  credentials.init();

  Serial.println("Loading credentials...");

  credentials.load();

  credentials.print();

  credentials.ssid.set("SchmoopyWeb-");
  credentials.password.set("Nikkuchan6969-");
  credentials.deviceid.set("some_device_id");
  credentials.username.set("nick");

  Serial.println("Changed credentials, printing again:");
  credentials.print();

  credentials.save();
  }

void loop() {
  // put your main code here, to run repeatedly:

}
