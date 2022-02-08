#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

#include "Credentials.h"
#include "OledDisplay.h"
#include "constants.h"
#include "SparkFun_SCD30_Arduino_Library.h"

#define FACTORY_RESET 13

bool configSaved = false; //flag that indicates that a user has saved their credentials through the web form

void handle_root();
void handle_saveconfig();

WebServer server(80);
Credentials credentials; //Holds credentials such as ssid, password, deviceid and username
OledDisplay display;

SCD30 airSensor;

unsigned long previousMillis = 0; //Keep track of milliseconds between server requests

//This function will create a Wi-Fi access point, and configuration web server
void createConfigurationPortal() {
    WiFi.softAP(ap_ssid, ap_password);

    //Modify the HTML to replace credentials if they have already been set
    html = String(rawTemplateHtml);
    html.replace("{SSID}", credentials.ssid.value);
    html.replace("{PASSWORD}", credentials.password.value);
    html.replace("{DEVICEID}", credentials.deviceid.value);
    html.replace("{USERNAME}", credentials.username.value);

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
}

bool connectToWifi(const char* ssid, const char* password, int timeoutSeconds) {
    Serial.println("Trying to connect to WiFi");
    display.connectingToWifi();
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    bool connectionSuccess = true;
    int timeoutCounter = 0;

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        timeoutCounter++;
        Serial.print(".");
        display.printDot();

        if (timeoutCounter > timeoutSeconds) {
            connectionSuccess = false;
            break;
        }
    }
    return connectionSuccess;
}

bool factoryReset() {
    pinMode(FACTORY_RESET, INPUT_PULLUP);
    delay(100);
    //While factory reset button is held down
    int resetCounter = 0;
    while(!digitalRead(FACTORY_RESET)) {
      resetCounter++;
      if(resetCounter > 20) {
          return true;
      }
      delay(100);
    }
    return false;
}

void setup() {
    Serial.begin(115200);

    credentials.init();
    display.init();
    display.printSplashScreen();

    if(factoryReset()) {
        display.writeText("Resetting to factory defaults...", 1);
        credentials.remove();
    }
    delay(2000);

    //Load credentials
    credentials.load();

    if(credentials.incomplete()) {
      Serial.println("Not all credentials exist, starting Wi-Fi access point and Web server...");
      display.writeText("Could not connect to\ndefault Wi-Fi network\n\nCreating Wi-Fi AP:\n\nSSID: CO2Sense\nPASS: password", 1);
      createConfigurationPortal();
      
      //Try to connect to wifi again now that new credentials have been filled in
      if(!connectToWifi(credentials.ssid.value, credentials.password.value, 10)) {
        Serial.println("\nCould not connect to wifi, please restart the device.");
        while(1){;}
      }
    } 
    //If all credentials are filled in, try to connect to wifi
    else { 
      if(!connectToWifi(credentials.ssid.value, credentials.password.value, 10)) {
        Serial.println("\nCould not connect to wifi, starting Wi-Fi access point and Web server...");
        display.writeText("Could not connect to\ndefault Wi-Fi network\n\nCreating Wi-Fi AP:\n\nSSID: CO2Sense\nPASS: password", 1);
        createConfigurationPortal();

        //Try to connect to wifi again now that new credentials have been filled in
        if(!connectToWifi(credentials.ssid.value, credentials.password.value, 10)) {
          Serial.println("\nCould not connect to wifi, please restart the device.");
          while(1){;}
        }
      }
    }

    Serial.println("\nConnected to Wifi!");
    display.successConnectedToWifi();
    delay(1000);

    Wire.begin();

    airSensor.begin(Wire, false);
    delay(250);
    airSensor.setTemperatureOffset(4.0);
}

void transmitToServer(uint16_t co2, String temperature, String humidity) {

  HTTPClient http;
  String serverURL = String(endpoint);

  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  String deviceid_s = String(credentials.deviceid.value);
  String username_s = String(credentials.username.value);

  String request = "{";
  request += "\"deviceid\":";
  request += "\"" + deviceid_s + "\",";
  request += "\"username\":";
  request += "\"" + username_s + "\",";
  request += "\"co2\":";
  request += "\"" + String(co2) + "\",";
  request += "\"temperature\":";
  request += "\"" + temperature + "\",";
  request += "\"humidity\":";
  request += "\"" + humidity + "\"}";

  Serial.print("Free heap memory: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("Sending: ");
  Serial.println(request);

  int http_response_code = http.POST(request);

  Serial.print("HTTP code: ");
  Serial.println(String(http_response_code));

  if (http_response_code < 0) {
      Serial.println("Did not get a reply from the server, trying to reconnect to wifi...");
      WiFi.disconnect();
      connectToWifi(credentials.ssid.value, credentials.password.value, 10);
  } else {
      String response = http.getString();
      Serial.print("Server response: ");
      Serial.println(response);
  }
  
  http.end();
}

void loop() {
  if (airSensor.dataAvailable()) {
    uint16_t co2 = airSensor.getCO2();
    display.printCO2value(co2);
    
    String temperature = String(airSensor.getTemperature(),1);
    String humidity = String(airSensor.getHumidity(),0);

    //If 10 seconds have passed since the last server request, then we can send again
    if(millis() - previousMillis > 10000) {

      previousMillis = millis();
      transmitToServer(co2, temperature, humidity);
    }
  }
  
  delay(500);
}


//This function is called when the user submits the configuration page
void handle_saveconfig() {
    Serial.println("Configuration has been sent by the user:");
    
    if (strlen(server.arg("ssid").c_str()) > 0) {
        credentials.ssid.set(server.arg("ssid").c_str());
        Serial.print("SSID: ");
        Serial.println(server.arg("ssid").c_str());
    }
    if (strlen(server.arg("password").c_str()) > 0) {
        credentials.password.set(server.arg("password").c_str());
        Serial.print("Password: ");
        Serial.println(server.arg("password").c_str());
    }
    if (strlen(server.arg("deviceid").c_str()) > 0) {
        credentials.deviceid.set(server.arg("deviceid").c_str());
        Serial.print("DeviceID: ");
        Serial.println(server.arg("deviceid").c_str());
    }
    if (strlen(server.arg("username").c_str()) > 0) {
        credentials.username.set(server.arg("username").c_str());
        Serial.print("Username: ");
        Serial.println(server.arg("username").c_str());
    }

    server.send(200, "text/html", htmlConfigurationSaved);
    configSaved = true;
    credentials.save();
}

void handle_root() { server.send(200, "text/html", html); }
