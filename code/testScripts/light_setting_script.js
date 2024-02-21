var minimum_brightness = 0;
var maximum_brightness = 100;
var minimum_temperature = 2700;
var maximum_temperature = 6500;

var brightness_slider = document.getElementById("brightness_slider");
var brightness_output = document.getElementById("brightness_value");
var temperature_slider = document.getElementById("temperature_slider");
var temperature_output = document.getElementById("temperature_value");
var toggle_button = document.getElementById("toggle_powered");
// var slidecontainer = document.getElementById("slidecontainer");
console.log(brightness_output.value);
brightness_output.value = brightness_slider.value;
temperature_output.value = temperature_slider.value;
brightness_slider.oninput = function () {
    brightness_output.value = this.value;
}
brightness_output.oninput = function () {
    if (this.value > maximum_brightness) {
        this.value = maximum_brightness;
    } else if (this.value < minimum_brightness) {
        this.value = minimum_brightness;
    }
    brightness_slider.value = this.value;
}
temperature_slider.oninput = function () {
    temperature_output.value = this.value;
}
temperature_output.oninput = function () {
    if (this.value > maximum_temperature) {
        this.value = maximum_temperature;
    } else if (this.value < minimum_temperature) {
        this.value = minimum_temperature;
    }
    temperature_slider.value = this.value;
}

function update(element) {
    var xhr = new XMLHttpRequest();
    if (element.id.includes("toggle")) {
        // console.log("toggled");
        if (element.checked) {
            xhr.open("GET", "/update?output=powered&state=1", true);
        } else {
            xhr.open("GET", "/update?output=powered&state=0", true);
        }
    } else {
        if (element.id.includes("brightness")) {
            xhr.open("GET", "/update?output=brightness&state=" + element.value, true);
        } else if (element.id.includes("temperature")) {
            xhr.open("GET", "/update?output=temperature&state=" + element.value, true);
        }
    }
    xhr.send();
}