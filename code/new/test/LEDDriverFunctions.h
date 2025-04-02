#pragma once
#include "Arduino.h"

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

void setupDriver(int cool_white, int warm_white, int frequency = 1000, int resolution = 8);
void updateBrightness();
void setBrightness(int cool_white_brightness, int warm_white_brightness);
void setCoolWhiteBrightness(int cool_white_brightness);
void setWarmWhiteBrightness(int warm_white_brightness);

