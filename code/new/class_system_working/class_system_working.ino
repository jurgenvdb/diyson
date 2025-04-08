#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "html.h"

class LEDInfo {
public:
  int cool_white_pin = 0;
  int warm_white_pin = 1;
  int cool_white_brightness = 0;
  int warm_white_brightness = 0;
  int freq = 1000;
  int res = 8;
} led_info;

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleUpdate() {
  if (server.method() == HTTP_POST) {
    if (server.hasArg("plain") == false || server.arg("plain").length() == 0) {
      server.send(400, "text/plain", "Bad Request: Missing JSON data");
      return;
    }
    String jsonString = server.arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
      server.send(400, "text/plain", "Bad Request: Invalid JSON");
      return;
    }
    led_info.cool_white_brightness = doc["cwValue"];
    led_info.warm_white_brightness = doc["wwValue"];
    server.send(200, "text/plain", "OK");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleGetValues() {
  DynamicJsonDocument doc(1024);
  doc["cwValue"] = led_info.cool_white_brightness;
  doc["wwValue"] = led_info.warm_white_brightness;
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}



class LEDDriver {
public:
  void setupDriver(int cool_white, int warm_white, int frequency = 1000, int resolution = 8) {
    led_info.cool_white_pin = cool_white;
    led_info.warm_white_pin = warm_white;
    led_info.freq = frequency;
    led_info.res = resolution;
    ledcAttach(led_info.cool_white_pin, led_info.freq, led_info.res);
    ledcAttach(led_info.warm_white_pin, led_info.freq, led_info.res);
  }

  void updateBrightness() {
    ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
    ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
  }

  void setBrightness(int cool_white_brightness, int warm_white_brightness) {
    led_info.cool_white_brightness = cool_white_brightness;
    led_info.warm_white_brightness = warm_white_brightness;
    ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
    ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
  }

  void setCoolWhiteBrightness(int cool_white_brightness) {
    led_info.cool_white_brightness = cool_white_brightness;
    ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
  }

  void setWarmWhiteBrightness(int warm_white_brightness) {
    led_info.warm_white_brightness = warm_white_brightness;
    ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
  }
} LEDdriver;

class Webserver {
public:
  IPAddress connectWiFi(char *ssid, char *password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi...");
    }
    return WiFi.localIP();
  }

  void setupWebServer() {
    server.onNotFound([]() {
      server.send(404, "text/plain", "Not Found");
    });
    server.on("/", handleRoot);
    server.on("/update_values", HTTP_POST, handleUpdate);
    server.on("/get_values", HTTP_GET, handleGetValues);
    server.begin();
  }

  void handleClient() {
    server.handleClient();
  }
} webserver;

void setup() {
  Serial.begin(115200);
  Serial.println(webserver.connectWiFi("Bibliotheek", "Vakantie"));
  webserver.setupWebServer();
  LEDdriver.setupDriver(0,1);
}

void loop() {
  webserver.handleClient();
  Serial.print("Cool white brightness: ");
  Serial.println(led_info.cool_white_brightness);
}
