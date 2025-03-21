#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

const char HTML[] =
R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Small test for web and driver</title>
</head>
<body>
    <form action="/get">
        <input name="brightness" type="number" min="0", max="255" value="0">
        <input type="submit">
    </form>
</body>
</html>
)=====";

AsyncWebServer server(80);
const char* ssid = "Habbo Hotel";
const char* password = "Qu!ck!3W33";

const char* PARAM_MESSAGE = "brightness";

const int ledPin0 = 0;
const int ledPin1 = 1;
const int onboardLED = 10;
const int freq = 1000;
const int resolution = 8;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    Serial.println("booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/hmtl", HTML);
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
            // Serial.println(message);
            Serial.println(millis());
            Serial.println(message.toInt());
            ledcWrite(ledPin0, message.toInt());
            ledcWrite(ledPin1, message.toInt());
            ledcWrite(onboardLED, message.toInt());
        } else {
            message = "No message sent";
        }
        request->send(200, "text/html", HTML);
    });

    server.onNotFound(notFound);

    // pinMode(onboardLED, OUTPUT);
    
    ledcAttach(ledPin0, freq, resolution);
    ledcAttach(ledPin1, freq, resolution);
    ledcAttach(onboardLED, freq, resolution);

    server.begin();
}

void loop() {
}
