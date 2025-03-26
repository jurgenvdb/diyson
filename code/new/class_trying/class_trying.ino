#include <webserver.h>
#include <ledDriver.h>

Webserver testServer("Habbo Hotel", "Qu!ck!3W33");
LEDDriver onboardLED(10);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println(testServer.getIP());

}

void loop() {
  Serial.println(testServer.getCoolWhiteDuty());
}
