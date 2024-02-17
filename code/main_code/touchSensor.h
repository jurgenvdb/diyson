#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

TouchState touch_state = {0, 0};
Adafruit_MPR121 cap = Adafruit_MPR121();
int brightness_values[touch_slider_length * 2 - 1];
int temperature_values[touch_slider_length * 2 - 1];

void setup_touch() {
    if(!cap.begin(touch_address)) {
        Serial.println("MPR121 not found, check wiring?");
        while(1);
    }
    for (int i = 0; i < touch_slider_length * 2 - 1; i++) {
        output_values[i] = round(maximum_brightness / (touch_slider_length * 2 - 1) * i);
    }
    for (int i = 0; i < touch_slider_length * 2 - 1; i++) {
        temperature_values[i] = round((maximum_temperature - minimum_temperature) / (touch_slider_length * 2 - 1) * i) + minimum_temperature;
    }
}

void is_power_touched() {
    if ((touch_state.current_touched & _BV(power_pad)) && !(touch_state.last_touched & _BV(power_pad))) {
        return true;
    }
}

void get_brightness() {
    for (int i = 0; i < touch_slider_length; i++) {
        if (touch_state.current_touched & _BV(brightness_pads[i])) {
            if (i != touch_slider_length - 1 && (touch_state.current_touched & _BV(brightness_pads[i + 1]))) {
                return brightness_values[i + 1];
            }
            return brightness_values[i];
        }
    }
}

void get_temperature() {
    for (int i = 0; i < touch_slider_length; i++) {
        if (touch_state.current_touched & _BV(temperature_pads[i])) {
            if (i != touch_slider_length - 1 && (touch_state.current_touched & _BV(temperature_pads[i + 1]))) {
                return temperature_values[i + 1];
            }
            return temperature_values[i];
        }
    }
}

void update_touch(LampState &lamp_state) {
    touch_state.current_touched = cap.touched();
    if (is_power_touched()) {
        lamp_state.powered = !lamp_state.powered;
    }
    lamp_state.brightness = get_brightness();
    lamp_state.temperature = get_temperature();
    
}