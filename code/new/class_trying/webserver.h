#ifndef webserver.h
#define webserver.h

#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

class Webserver {
  private:
    String html;
    WebServer server;
    int warmWhiteDuty = 255;
    int coolWhiteDuty = 255;
    IPAddress IP;

  public:
    Webserver(const char* ssid, const char* password);
    int getCoolWhiteDuty();
    int getWarmWhiteDuty();
    void handleRoot();
    void handleUpdate();
    IPAddress getIP();
};

#endif