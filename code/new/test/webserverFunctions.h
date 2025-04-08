#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "html.h"
#include "LEDInfo.h"

IPAddress connectWiFi(char *ssid, char *password);
void handleRoot();
void handleUpdate();
void handleGetValues();
void setupWebServer();
void handleClient();