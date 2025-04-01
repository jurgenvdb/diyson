#ifndef WEBSERVERFUNCTIONS_CPP
  #include "webserverFunctions.cpp"
#endif
// #ifndef LEDDRIVERFUNCTIONS_CPP
//   #include "LEDDriverFunctions.cpp"
// #endif

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println(connectWiFi("Oneplus 12", "n58btsjv"));
  setupWebServer();
  setupDriver(0, 1);
}

void loop() {
  handleClient();
  Serial.print("Cool white brightness: ");
  Serial.println(led_info.cool_white_brightness);
}
