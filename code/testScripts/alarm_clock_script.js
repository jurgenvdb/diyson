var minimum_brightness = 0;
var maximum_brightness = 100;
var minimum_temperature = 2000;
var maximum_temperature = 6500;
var minimum_time = 0;
var maximum_time = 10;

var alarm_brightness_slider = document.getElementById("alarm_brightness");
var alarm_brightness_output = document.getElementById("alarm_brightness_value");
var alarm_temperature_slider = document.getElementById("alarm_temperature");
var alarm_temperature_output = document.getElementById("alarm_temperature_value");
var alarm_fade_slider = document.getElementById("alarm_fade");
var alarm_fade_output = document.getElementById("alarm_fade_value");
alarm_brightness_output.value = alarm_brightness_slider.value;
alarm_temperature_output.value = alarm_temperature_slider.value;
alarm_fade_output.value = alarm_fade_slider.value;
alarm_brightness_slider.oninput = function () {
    alarm_brightness_output.value = this.value;
}
alarm_brightness_output.oninput = function () {
    if (this.value > maximum_brightness) {
        this.value = maximum_brightness;
    } else if (this.value < minimum_brightness) {
        this.value = minimum_brightness;
    }
    alarm_brightness_slider.value = this.value;
}
alarm_temperature_slider.oninput = function () {
    alarm_temperature_output.value = this.value;
}
alarm_temperature_output.oninput = function () {
    if (this.value > maximum_temperature) {
        this.value = maximum_temperature;
    } else if (this.value < minimum_temperature) {
        this.value = minimum_temperature;
    }
    alarm_temperature_slider.value = this.value;
}
alarm_fade_slider.oninput = function () {
    alarm_fade_output.value = this.value;
}
alarm_fade_output.oninput = function () {
    if (this.value > maximum_time) {
        this.value = maximum_time;
    } else if (this.value < minimum_time) {
        this.value = minimum_time;
    }
    alarm_fade_slider.value = this.value;
}