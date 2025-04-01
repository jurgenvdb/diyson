#include "Arduino.h"
#define LEDDRIVERFUNCTIONS_CPP

class LEDInfo
{
public:
    int cool_white_pin = 0;
    int warm_white_pin = 1;
    int cool_white_brightness = 0;
    int warm_white_brightness = 0;
    int freq = 1000;
    int res = 8;
};

LEDInfo led_info;

void setupDriver(int cool_white, int warm_white, int frequency = 1000, int resolution = 8)
{
    led_info.cool_white_pin = cool_white;
    led_info.warm_white_pin = warm_white;
    led_info.freq = frequency;
    led_info.res = resolution;
    ledcAttach(led_info.cool_white_pin, led_info.freq, led_info.res);
    ledcAttach(led_info.warm_white_pin, led_info.freq, led_info.res);
}

void updateBrightness() {
  ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
  ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
}

void setBrightness(int cool_white_brightness, int warm_white_brightness) {
    led_info.cool_white_brightness = cool_white_brightness;
    led_info.warm_white_brightness = warm_white_brightness;
    ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
    ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
}

void setCoolWhiteBrightness(int cool_white_brightness) {
    led_info.cool_white_brightness = cool_white_brightness;
    ledcWrite(led_info.cool_white_pin, led_info.cool_white_brightness);
}

void setWarmWhiteBrightness(int warm_white_brightness) {
    led_info.warm_white_brightness = warm_white_brightness;
    ledcWrite(led_info.warm_white_pin, led_info.warm_white_brightness);
}

