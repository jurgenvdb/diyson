#include "LEDInfo.h"
#include "LEDDriverFunctions.h"
#include "webserverFunctions.h"


char* ssid = "Bibliotheek";
char* pass = "Vakantie";
// char* ssid = "Oneplus 12";
// char* pass = "n58btsjv";
// char* ssid = "Habbo Hotel";
// char* ssid = "Qu!ck!3W33"l

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println(connectWiFi(ssid, pass));
  setupWebServer();
  setupDriver(0, 1);
}

void loop() {
  handleClient();
  Serial.print("Cool white brightness: ");
  Serial.println(led_info.cool_white_brightness);
}
