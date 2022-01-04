#include "Credentials.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

StringParameter::StringParameter(int arrayLength) {
    length = arrayLength;
    value = new char[length]{};
}

void StringParameter::set(const char* value) {
    strlcpy(this->value, value, length);
}

Credentials::Credentials() {

}

void Credentials::init() { SPIFFS.begin(true); }

void Credentials::load()
{
    File file = SPIFFS.open("/credentials.json");

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println("credentials.json did not exist, setting default credentials");
    }

    //Parameters will default to "" if not set
    strlcpy(ssid.value, doc["ssid"] | "", ssid.length);
    strlcpy(password.value, doc["password"] | "", password.length);
    strlcpy(deviceid.value, doc["deviceid"] | "", deviceid.length);
    strlcpy(username.value, doc["username"] | "", username.length);

    file.close();
}
void Credentials::save()
{
    SPIFFS.remove("/credentials.json");

    File file = SPIFFS.open("/credentials.json", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to create file");
        return;
    }

    StaticJsonDocument<256> doc;

    doc["ssid"] = ssid.value;
    doc["password"] = password.value;
    doc["deviceid"] = deviceid.value;
    doc["username"] = username.value;

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write to file");
    }

    file.close();
}

//Returns True if not all credentials are filled in
bool Credentials::incomplete() {
    if(strlen(ssid.value) == 0 || 
    strlen(password.value) == 0 || 
    strlen(deviceid.value) == 0 || 
    strlen(username.value) == 0) {
        return true;
    } else {
        return false;
    }
}

void Credentials::print() {
    Serial.print("SSID: ");
    Serial.println(ssid.value);
    Serial.print("Password: ");
    Serial.println(password.value);
    Serial.print("DeviceID: ");
    Serial.println(deviceid.value);
    Serial.print("Username: ");
    Serial.println(username.value);
}

void Credentials::remove() {
    SPIFFS.remove("/credentials.json");
}