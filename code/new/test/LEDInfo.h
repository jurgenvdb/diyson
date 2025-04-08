#pragma once

class LEDInfo
{
public:
    int cool_white_pin = 0;
    int warm_white_pin = 1;
    int cool_white_brightness = 0;
    int warm_white_brightness = 0;
    int freq = 1000;
    int res = 8;
} led_info;