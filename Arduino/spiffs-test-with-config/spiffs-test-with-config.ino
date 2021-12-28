#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <SPI.h>
#include "constants.h"

void loadCredentials() {
  File file = SPIFFS.open("/credentials.json");

  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println("credentials.json did not exist, using default configuration");

  strncpy(credentials.ssid, doc["ssid"] | "", sizeof(credentials.ssid)); 
  strncpy(credentials.password, doc["password"] | "", sizeof(credentials.password)); 
  strncpy(credentials.deviceid, doc["deviceid"] | "", sizeof(credentials.deviceid)); 
  strncpy(credentials.username, doc["username"] | "", sizeof(credentials.username)); 

  file.close();
}

void saveCredentials() {
  SPIFFS.remove("/credentials.json");

  File file = SPIFFS.open("/credentials.json", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to create file");
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["ssid"] = credentials.ssid;
  doc["password"] = credentials.password;
  doc["deviceid"] = credentials.deviceid;
  doc["username"] = credentials.username;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write to file");
  }

  file.close();
}

void setup() {
    Serial.begin(115200);

    SPIFFS.begin();
    //SPIFFS.remove("/credentials.json");
    //SPIFFS.format();
    //Serial.println("Done formatting.");
    //return;

    loadCredentials();

    Serial.println("Credentials:");
    Serial.println(credentials.ssid);
    Serial.println(credentials.password);
    Serial.println(credentials.deviceid);
    Serial.println(credentials.username);
    Serial.println("-End of credentials-");

    if(strlen(credentials.ssid) == 0 || strlen(credentials.password) == 0 || strlen(credentials.deviceid) == 0 || strlen(credentials.username) == 0) {
      Serial.println("Not all credentials are saved, we should start the web portal.");
    }

    strncpy(credentials.username, "NickLeijenhorst", sizeof(credentials.username)); 

    saveCredentials();
}
void loop() {}
