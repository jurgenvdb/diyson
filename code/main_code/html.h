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

struct htmlPiece {
  
};

const char main_html[] = 
R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@24,400,0,0" />
</head>

<style>
    :root {
        --main-color: #0d3739;
        --secondary-color: #353434;
        /* --tertiary-color: #DFD3C3;
        --quaternary-color: #F0ECE3; */
        --text-color: #f2f2f2;
    }
</style>

<style>
    body {
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 0;
        background-color: var(--main-color);
    }

    #main_controls {
        color: var(--text-color);
        width: 100%;
        text-align: center;
    }
    
    #main_controls h3 {
        font-size: 2vh;
    }

    #main_controls table {
        width: 100%;
    }

    #main_controls table .checkbox {
        width: 100%;
        height: 80vh;
    }
    
    #main_controls .slider {
        height: 80vh;
        appearance: slider-vertical;
    }

    
    
    #alarm_clock {
        background-color: var(--primary-color);
        color: var(--text-color);
    }

    #settings {
        background-color: var(--primary-color);
        color: var(--text-color);
    }
    
    
    
    #bottom_menu {
        position: fixed;
        bottom: 0;
        width: 100%;
        background-color: var(--secondary-color);
        color: var(--text-color);
        overflow: auto;
        height: 6vh;
    }

    #bottom_menu a {
        float: left;
        text-decoration: none;
        font-size: 17px;
        width: 33.33%;
        text-align: center;
        line-height: 6vh;
        color: var(--text-color);
    }

    #bottom_menu a:hover {
        background-color: var(--text-color);
        color: var(--main-color);
    }
</style>

<body>
    <div id="main_controls" style="display: block;">
        <h3>Main Control</h3>
        <table>
            <tr class="slider_row">
                <td><input type="range" orient="vertical" min="10" max="100" value="%PLACEHOLDER_BRIGHTNESS%" class="slider" id="brightness_slider" onchange="update(this)"></td>
                <td><input type="checkbox" id="toggle_powered" class="checkbox" onchange="update(this)" %PLACEHOLDER_POWERED%></td>
                <td><input type="range" min="2700" max="6500" value="%PLACEHOLDER_TEMPERATURE%" class="slider" id="temperature_slider" onchange="update(this)"></td>
            </tr>
            <tr class="value_row">
                <td><input type="number" min="10" max="100" value="%PLACEHOLDER_BRIGHTNESS%" id="brightness_value" onchange="update(this)"></td>
                <td></td>
                <td><input type="number" min="2700" max="6500" value="%PLACEHOLDER_TEMPERATURE%" id="temperature_value" onchange="update(this)"></td>
            </tr>
        </table>
    </div>

    <!-- <br><br><br><br><br> -->

    <div id="alarm_clock" style="display: none;">
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
                        <td><input type="range" min="10" max="100" value="%PLACEHOLDER_BRIGHTNESS%" class="slider"
                                id="alarm_brightness" name="alarm_brightness_slider"></td>
                        <td><input type="number" id="alarm_brightness_value" name="alarm_brightness_output"
                                value="%PLACEHOLDER_BRIGHTNESS%" min="10" max="100"><br></td>
                    </tr>
                    <tr>
                        <th align="left">Temperature</th>
                        <td><input type="range" min="2700" max="6500" value="%PLACEHOLDER_TEMPERATURE%" class="slider"
                                id="alarm_temperature" name="alarm_temperature_slider"></td>
                        <td><input type="number" id="alarm_temperature_value" name="alarm_temperature_output"
                                value="%PLACEHOLDER_TEMPERATURE%" min="2700" max="6500"></td>
                    </tr>
                    <tr>
                        <th align="left">Time</th>
                        <td><input type="range" min="0" max="10" value="2" class="slider" , id="alarm_fade"
                                name="alarm_fade"></td>
                        <td><input type="number" min="0" max="10" value="2" id="alarm_fade_value"
                                name="alarm_fade_value"></td>
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

    <!-- <br><br><br><br><br> -->

    <div id="settings" style="display: none;">
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

    <div id="bottom_menu" name="bottom_menu">
        
        <a href="#main_controls" onclick=switchVisible(this)><span class="material-symbols-outlined">home</span></a>
        <a href="#alarm_clock" onclick=switchVisible(this)><span class="material-symbols-outlined">alarm</span></a>
        <a href="#settings" onclick=switchVisible(this)><span class="material-symbols-outlined">settings</span></a>
    </div>

    <script name="light_settings_script">
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

    <script name="navigation_script">
        var main_controls = document.getElementById("main_controls");
        var alarm_clock = document.getElementById("alarm_clock");
        var settings = document.getElementById("settings");

        function switchVisible(element) {
            console.log("switchVisible")
            console.log(element.href.split("#")[1]);
            if (element.href.split("#")[1] == main_controls.id) {
                console.log("main_controls");
                main_controls.style.display = "block";
                alarm_clock.style.display = "none";
                settings.style.display = "none";
            } else if (element.href.split("#")[1] == alarm_clock.id) {
                console.log("alarm_clock");
                main_controls.style.display = "none";
                alarm_clock.style.display = "block";
                settings.style.display = "none";
            } else if (element.href.split("#")[1] == settings.id) {
                console.log("settings");
                main_controls.style.display = "none";
                alarm_clock.style.display = "none";
                settings.style.display = "block";
            }
        }
    </script>
</body>

</html>
)=====";