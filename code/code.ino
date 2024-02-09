#include "webServer.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);
  EEPROM.begin(500);

  wifi_configure("Habbo Hotel", "Qu!ck!3W33");

  if (!wifi_is_configured()) {
    Serial.println("Setting up configuration server");
    wifi_setup_configure_server();
    while (!wifi_configure_server_handler());
  }
  Serial.println("Network credentials found, attempting to connect");
  wifi_connect();
  setup_main_server();
}

void loop() {
  
}



// #include <EEPROM.h>

// void setup() {
//   Serial.begin(115200);
//   while(!Serial);

//   EEPROM.begin(2048);

//   int val = byte(random(10020));
//   EEPROM.writeInt(1,val);

//   Serial.printf("Read At 1 boot: %d\n", EEPROM.readInt(1));// return value is normal
// }

// void loop() {

// }