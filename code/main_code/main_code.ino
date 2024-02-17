#include "settings.h"
#include "webServer.h"
// #include "touchSensor.h"
// #include "ledDriver.h"

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
  // setup_drivers();
  // setup_touch();
}

void loop() {
  // update_touch(lamp_state);
  // set_driver(lamp_state);
  // delay(main_loop_delay);
}