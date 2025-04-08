#pragma once
#include "Arduino.h"
#include "LEDInfo.h"

void setupDriver(int cool_white, int warm_white, int frequency = 1000, int resolution = 8);
void updateBrightness();
void setBrightness(int cool_white_brightness, int warm_white_brightness);
void setCoolWhiteBrightness(int cool_white_brightness);
void setWarmWhiteBrightness(int warm_white_brightness);

