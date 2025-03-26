#include "ledDriver.h"
#include "esp32-hal-ledc.h"
#include "Arduino.h"

LEDDriver::LEDDriver(int controlPin) {
  pin = pin;
  ledcAttach(pin, frequency, resolution);
}

void LEDDriver::setDuty(int dutyCycle) {
  duty = dutyCycle;
  ledcWrite(pin, duty);
}

int LEDDriver::getDuty() {
  return duty;
}

void LEDDriver::turnOFF() {
  duty = 0;
  ledcWrite(pin, 0);
}