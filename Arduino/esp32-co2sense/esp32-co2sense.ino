#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <SPI.h>

#include "constants.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool configSaved = false;

void handle_root();
void send_bootstrap();
void handle_saveconfig();

WebServer server(80);

void loadCredentials() {
  File file = SPIFFS.open("/credentials.json");

  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println("credentials.json did not exist, using default configuration");

  strlcpy(credentials.ssid, doc["ssid"] | "", sizeof(credentials.ssid)); 
  strlcpy(credentials.password, doc["password"] | "", sizeof(credentials.password)); 
  strlcpy(credentials.deviceid, doc["deviceid"] | "default-deviceid", sizeof(credentials.deviceid)); 
  strlcpy(credentials.username, doc["username"] | "default-username", sizeof(credentials.username)); 

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

void writeTextToScreen(const char *text, uint8_t textSize = 1) {
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.write(text);
    display.display();
}

void autoConnectToWifi() {
    Serial.println("Trying to connect to Wifi with stored credentials");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 25);
    display.cp437(true);
    display.write("Connecting to Wi-Fi\n");
    display.display();

    WiFi.begin();

    int timeoutCounter = 0;
    bool connectionSuccess = true;

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        timeoutCounter++;
        Serial.print(".");
        display.write(".");
        display.display();

        if (timeoutCounter > 20) {
            connectionSuccess = false;
            break;
        }
    }

    if (connectionSuccess) {
        Serial.println("CONNECTION SUCCESSFUL");
        return;
    }

    // This point is reached if the default wifi connection didn't work

    Serial.println("\nCould not connect, starting as access point");
    writeTextToScreen(
        "Could not connect to\ndefault Wi-Fi network\n\nCreating Wi-Fi "
        "AP:\n\nSSID: "
        "CO2Sense\nPASS: password",
        1);
    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("Starting webserver...");
    server.on("/", handle_root);
    server.on("/saveconfig", handle_saveconfig);
    server.begin();

    bool shouldStop = false;
    unsigned long timeConfigSaved = 0;

    while (1) {
        server.handleClient();

        if (configSaved) {
            configSaved = false;
            shouldStop = true;
            timeConfigSaved = millis();
        }

        if (shouldStop && (millis() > (timeConfigSaved + 2000))) {
            Serial.println("Shutting down server");
            server.close();
            break;
        }
    }

    Serial.print("Trying to connect to wifi");
    writeTextToScreen("Connecting to WiFi", 1);

    WiFi.begin(credentials.ssid, credentials.password);

    timeoutCounter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        timeoutCounter++;
        Serial.print(".");

        if (timeoutCounter > 10) {
            Serial.println(
                "\nCan't connect to wifi, please restart the device and try "
                "again.");
            while (1) {
                ;
            }
        }
    }
}

void printSplashScreen() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(18, 10);
    display.cp437(true);
    display.print("CO2Sense");

    display.setCursor(0, 45);
    display.setTextSize(1);
    display.print("Developed by:");
    display.setCursor(0, 55);
    display.print("Nick Leijenhorst");
    display.display();
}

void setup() {
    Serial.begin(115200);
    SPIFFS.begin();
    loadCredentials();

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }

    if(strlen(credentials.ssid) == 0 || strlen(credentials.password) == 0 || strlen(credentials.deviceid) == 0 || strlen(credentials.username) == 0) {
      Serial.println("Not all credentials are saved, we should start the web portal.");
    } else {
      Serial.println("Credentials:");
      Serial.println(credentials.ssid);
      Serial.println(credentials.password);
      Serial.println(credentials.deviceid);
      Serial.println(credentials.username);
      Serial.println("-End of credentials-");
    }

    printSplashScreen();
    delay(5000);

    autoConnectToWifi(); //Will block until wifi is connected successfully

    //WiFi.disconnect(true, true);

    Serial.println("Connected!");
    writeTextToScreen("\nConnected!", 2);
}

void loop() {}


//This function is called when the user submits the configuration page
void handle_saveconfig() {
    Serial.println("Configuration has been sent by the user:");
    
    if (strlen(server.arg("ssid").c_str()) > 0) {
        strlcpy(credentials.ssid, server.arg("ssid").c_str(), sizeof(credentials.ssid));
        Serial.print("SSID: ");
        Serial.println(credentials.ssid);
    }
    if (strlen(server.arg("password").c_str()) > 0) {
        strlcpy(credentials.password, server.arg("password").c_str(), sizeof(credentials.password));
        Serial.print("Password: ");
        Serial.println(credentials.password);
    }
    if (strlen(server.arg("deviceid").c_str()) > 0) {
        strlcpy(credentials.deviceid, server.arg("deviceid").c_str(), sizeof(credentials.deviceid));
        Serial.print("DeviceID: ");
        Serial.println(credentials.deviceid);
    }
    if (strlen(server.arg("username").c_str()) > 0) {
        strlcpy(credentials.username, server.arg("username").c_str(), sizeof(credentials.username));
        Serial.print("Username: ");
        Serial.println(credentials.username);
    }

    server.send(200, "text/html", htmlConfigurationSaved);
    configSaved = true;
    writeTextToScreen("  Config\n  Saved!", 2);
    saveCredentials();
}

void handle_root() { server.send(200, "text/html", html); }
