#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>

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
    Serial.print("Trying to connect to Wifi with stored credentials");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 25);
    display.cp437(true);
    display.write("Connecting to Wi-Fi\n");
    display.display();

    WiFi.mode(WIFI_STA);
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
            Serial.println("Configuration Saved!");
            Serial.println(ssid);
            Serial.println(password);
            writeTextToScreen("  Config\n  Saved!", 2);
        }

        if (shouldStop && (millis() > (timeConfigSaved + 2000))) {
            Serial.println("Shutting down server");
            server.close();
            break;
        }
    }

    Serial.print("Trying to connect to wifi");

    writeTextToScreen("Connecting to WiFi", 1);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, password);

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

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }

    printSplashScreen();
    delay(3000);

    autoConnectToWifi();

    writeTextToScreen("Connected!", 2);

    Serial.println("\nConnected to Wifi!");
    Serial.println(WiFi.localIP());

    WiFi.disconnect(true, true);
}

void loop() {}

void handle_saveconfig() {
    strcpy(ssid, server.arg("ssid").c_str());
    strcpy(password, server.arg("password").c_str());
    server.send(200, "text/html", htmlConfigurationSaved);
    configSaved = true;
}

void handle_root() { server.send(200, "text/html", html); }