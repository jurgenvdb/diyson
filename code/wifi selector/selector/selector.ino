#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "selectorhtml.h"

const char *ssid = "Diyson";
const char *password = "Diyson123";

WebServer server(80);

void handleRoot() {
  Serial.println("Loading Root");
  server.send(200, "text/html", selectorHTML);
}

void handleGetWifiSignals() {
  Serial.println("Handling network scan");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    server.send(400, "text/plain", "Error retrieving networks");
  } else {
    DynamicJsonDocument doc(1024);
    for (int i = 0; i < n; i++) {
      if (!doc.containsKey(WiFi.SSID(i).c_str())) {
        Serial.println(WiFi.SSID(i).c_str());
        switch (WiFi.encryptionType(i)) {
          case WIFI_AUTH_OPEN: doc[WiFi.SSID(i).c_str()] = "open"; break;
          case WIFI_AUTH_WEP: doc[WiFi.SSID(i).c_str()] = "WEP"; break;
          case WIFI_AUTH_WPA_PSK: doc[WiFi.SSID(i).c_str()] = "WPA"; break;
          case WIFI_AUTH_WPA2_PSK: doc[WiFi.SSID(i).c_str()] = "WPA2"; break;
          case WIFI_AUTH_WPA_WPA2_PSK: doc[WiFi.SSID(i).c_str()] = "WPA+WPA2"; break;
          case WIFI_AUTH_WPA2_ENTERPRISE: doc[WiFi.SSID(i).c_str()] = "WPA2-EAP"; break;
          case WIFI_AUTH_WPA3_PSK: doc[WiFi.SSID(i).c_str()] = "WPA3"; break;
          case WIFI_AUTH_WPA2_WPA3_PSK: doc[WiFi.SSID(i).c_str()] = "WPA2+WPA3"; break;
          case WIFI_AUTH_WAPI_PSK: doc[WiFi.SSID(i).c_str()] = "WAPI"; break;
          default: doc[WiFi.SSID(i).c_str()] = "unknown";
        }
      }
    }
    WiFi.scanDelete();
    String response;
    serializeJson(doc, response);
    Serial.println(response);
    server.send(200, "application/json", response);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting AP");
  WiFi.softAP(ssid, password);
  IPAddress myIP =  WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/get_wifi_signals", handleGetWifiSignals);
  server.begin();
}

void loop() {
  server.handleClient();
  // Serial.println("waiting...");
}
