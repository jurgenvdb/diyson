#ifndef ledDriver.h
#define ledDriver.h

#include "esp32-hal-ledc.h"
#include "Arduino.h"

class LEDDriver {
  private:
    int pin;
    int duty = 0;
    int frequency = 1000;
    int resolution = 8;
  
  public:
    LEDDriver(int controlPin);
    void setDuty(int dutyCycle);
    int getDuty();
    void turnOFF();
};

#endif