const String config_html[] = {
R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Document</title>
</head>
<body>
  <p>No network connected, please select a network and connect to it.</p>
  <label>SSID:</label>
  <select id="ssid">
)=====",
R"=====(
  </select>
  <br>
  <label>Password:</label>
  <input type="password" id="password">
  <br>
  <button onclick="connect()">Connect</button>
  <br>
  <p id="status"></p>

  <script>
    function connect() {
      var ssid = document.getElementById("ssid").value;
      var password = document.getElementById("password").value;
      window.location.href = window.location + "?ssid=" + ssid + "&password=" + password + "~";
    }
          
  </script>
</body>
</html>
)====="};

const char main_html[] = 
R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<style>
    /*  */
</style>

<body>
    <div id="main_controls">
        <h3>Main Controls:</h3>
        <table>
            <tr>
                <th align="left">On</th>
                <td><input type="checkbox" id="toggle" name="toggle" onchange="update(this)" %PLACEHOLDER_POWERED%></td>
            </tr>
            <tr>
                <th align="left">Brightness</th>
                <td><input type="range" min="10" max="100" value="%PLACEHOLDER_BRIGHTNESS%" class="slider"
                        id="brightness" name="brightness_slider" onchange="update(this)"></td>
                <td><input type="number" id="brightnessValue" name="brightness_output" value="%PLACEHOLDER_BRIGHTNESS%"
                        min="0" max="255"><br></td>
            </tr>
            <tr>
                <th align="left">Temperature</th>
                <td><input type="range" min="2700" max="6500" value="%PLACEHOLDER_TEMPERATURE%" class="slider"
                        id="temperature" name="temperature_slider" onchange="update(this)"></td>
                <td><input type="number" id="temperatureValue" name="temperature_output"
                        value="%PLACEHOLDER_TEMPERATURE%" min="0" max="255"></td>

        </table>
    </div>

    <br><br><br><br><br>

    <div id="alarm_clock">
        <div>
            <h4>Set A New Timer</h4>
            <form id="timer_settings">
                <label for="time">Time: </label>
                <input type="time" id="alarm_time" name="alarm_time">
                <br>
                <label for="repeat">Repeat: </label>
                <table>
                    <tr>
                        <th>S</th>
                        <th>M</th>
                        <th>T</th>
                        <th>W</th>
                        <th>T</th>
                        <th>F</th>
                        <th>S</th>
                    <tr>
                        <td><input type="checkbox" id="repeat_sunday" name="repeat_sunday"></td>
                        <td><input type="checkbox" id="repeat_monday" name="repeat_monday"></td>
                        <td><input type="checkbox" id="repeat_tuesday" name="repeat_tuesday"></td>
                        <td><input type="checkbox" id="repeat_wednesday" name="repeat_wednesday"></td>
                        <td><input type="checkbox" id="repeat_thursday" name="repeat_thursday"></td>
                        <td><input type="checkbox" id="repeat_friday" name="repeat_friday"></td>
                        <td><input type="checkbox" id="repeat_saturday" name="repeat_saturday"></td>
                    </tr>
                </table>
                <label for="light_settings">Brightness and temperature: </label>
                <table>
                    <tr>
                        <th align="left">Brightness</th>
                        <td><input type="range" min="10" max="100" value="%PLACEHOLDER_BRIGHTNESS%" class="slider" id="alarm_brightness"
                                name="alarm_brightness_slider"></td>
                        <td><input type="number" id="alarm_brightness_value" name="alarm_brightness_output" value="%PLACEHOLDER_BRIGHTNESS%"
                                min="10" max="100"><br></td>
                    </tr>
                    <tr>
                        <th align="left">Temperature</th>
                        <td><input type="range" min="2700" max="6500" value="%PLACEHOLDER_TEMPERATURE%" class="slider" id="alarm_temperature"
                                name="alarm_temperature_slider"></td>
                        <td><input type="number" id="alarm_temperature_value" name="alarm_temperature_output"
                                value="%PLACEHOLDER_TEMPERATURE%" min="2700" max="6500"></td>
                    </tr>
                    <tr>
                        <th align="left">Time</th>
                        <td><input type="range" min="0" max="10" value="2" class="slider", id="alarm_fade" name="alarm_fade"></td>
                        <td><input type="number" min="0" max="10" value="2" id="alarm_fade_value" name="alarm_fade_value"></td>
                    </tr>
                </table>
                <button id="submit" onclick="submit()">Save</button>
            </form>
        </div>
        <br><br>
        <div>
            <h4>Set Timers</h4>
            <table id="set_timers">
                <tr>
                    <th>Time</th>
                    <th>Date</th>
                    <th>Repeat</th>
                    <th>Remove</th>
                </tr>
                <tr>
                    <td>12:00</td>
                    <td>12/12/2020</td>
                    <td>Yes</td>
                    <td><button id="remove" onclick="removeTimer(this)">Remove</button></td>
                </tr>
            </table>
        </div>
    </div>

    <br><br><br><br><br>

    <div id="settings">
        <h4>Network Settings</h4>
        <form id="network_settings">
            <table>
                <tr>
                    <th align="left">SSID: </th>
                    <td><input type="text" , id="ssid" , name="ssid" , placeholder="SSID"></td>
                </tr>
                <tr>
                    <th align="left">Password: </th>
                    <td><input type="text" , id="password" , name="password" , placeholder="Password"></td>
                </tr>
            </table>
            <button id="submit" onclick="submit()">Submit</button>
        </form>
    </div>

    <script name="light_settings_script">
        var minimum_brightness = 0;
        var maximum_brightness = 100;
        var minimum_temperature = 2700;
        var maximum_temperature = 6500;

        var brightness_slider = document.getElementById("brightness");
        var brightness_output = document.getElementById("brightnessValue");
        var temperature_slider = document.getElementById("temperature");
        var temperature_output = document.getElementById("temperatureValue");
        var toggle_button = document.getElementById("toggle");
        var slidecontainer = document.getElementById("slidecontainer");
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
            if (element.id == "toggle") {
                // console.log("toggled");
                if (element.checked) {
                    xhr.open("GET", "/update?output=powered&state=1", true);
                } else {
                    xhr.open("GET", "/update?output=powered&state=0", true);
                }
            } else {
                if (element.name.includes("brightness")) {
                    xhr.open("GET", "/update?output=brightness&state=" + element.value, true);
                } else if (element.name.includes("temperature")) {
                    xhr.open("GET", "/update?output=temperature&state=" + element.value, true);
                }
            }
            xhr.send();
        }

    </script>

    <script name="alarm_clock_script">
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
    </script>
</body>

</html>
)=====";