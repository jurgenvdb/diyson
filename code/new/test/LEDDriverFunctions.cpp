#include "Arduino.h"
// #include "LEDInfo.h"
#include "LEDDriverFunctions.h"

void setupDriver(int cool_white, int warm_white, int frequency, int resolution)
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

