#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "html.h"
// #ifndef LEDDRIVERFUNCTIONS_cpp
  #include "LEDDriverFunctions.cpp"
// #endif

#define WEBSERVERFUNCTIONS_CPP

WebServer server(80);

IPAddress connectWiFi(char *ssid, char *password)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }
    return WiFi.localIP();
}

void handleRoot()
{
    server.send(200, "text/html", html);
}

void handleUpdate()
{
    if (server.method() == HTTP_POST)
    {
        if (server.hasArg("plain") == false || server.arg("plain").length() == 0)
        {
            server.send(400, "text/plain", "Bad Request: Missing JSON data");
            return;
        }

        String jsonString = server.arg("plain");
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, jsonString);
        if (error)
        {
            server.send(400, "text/plain", "Bad Request: Invalid JSON");
            return;
        }

        led_info.cool_white_brightness = doc["cwValue"];
        led_info.warm_white_brightness = doc["wwValue"];
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(405, "text/plain", "Method Not Allowed");
    }
}

void handleGetValues()
{
    DynamicJsonDocument doc(1024);
    doc["cwValue"] = led_info.cool_white_brightness;
    doc["wwValue"] = led_info.warm_white_brightness;
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void setupWebServer() 
{
    server.onNotFound([]() {
        server.send(404, "text/plain", "Not Found");
    });
    server.on("/", handleRoot);
    server.on("/update_values", HTTP_POST, handleUpdate);
    server.on("/get_values", HTTP_GET, handleGetValues);
    server.begin();
}

void handleClient() 
{
    server.handleClient();
}