#include "WiFiType.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

struct lampSettings {
  bool powered;
  int brightness;
  int temperature;
};

lampSettings lampState = {true, 100, 200};

#define ssidAddr 0
#define passAddr 250

#define mdns_name String("DIYson")
#define defaultSsid String("DIYson lamp project")
#define connection_timeout 30000

WiFiServer conf_server(80);

unsigned long scanInterval = 5000;
unsigned long lastScanUpdate = -scanInterval;
String networks = "";
bool configuration_server = false;

bool connection_established = false;

const char HTMLPart1[] =
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
)=====";

const char HTMLPart2[] =
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
)=====";

void wifi_configure(String ssid, String password) {
  EEPROM.writeUInt(ssidAddr, ssid.length());
  EEPROM.writeString(ssidAddr + 4, ssid);
  EEPROM.writeUInt(passAddr, password.length());
  EEPROM.writeString(passAddr + 4, password);
}

bool wifi_is_configured() {
  uint ssidLength = EEPROM.readUInt(ssidAddr);
  uint passLength = EEPROM.readUInt(passAddr);
  // Serial.print("configured ssid name length: ");
  // Serial.println(ssidLength);
  // Serial.print("configured password length: ");
  // Serial.println(passLength);
  if (ssidLength < 255 && passLength < 255) {
    return true;
  }
  return false;
}

void update_network_scan() {
  if (millis() > lastScanUpdate + scanInterval) {
    lastScanUpdate = millis();
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1) {
      Serial.println("Couldn't get a list of networks");
      return;
    }
    // Serial.print("Number of networks found: ");
    // Serial.println(numSsid);
    networks = "";
    for(int i = 0; i < numSsid; i++) {
      // Serial.println(WiFi.SSID(i));
      networks += WiFi.SSID(i);
      networks += "~";
    }
    networks = networks.substring(0, networks.length() - 1);
  }
}

void wifi_setup_configure_server() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(defaultSsid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);
  if(!MDNS.begin(mdns_name)) {
    Serial.println("Something went wrong with the mdns responder");
    while (true);
  }
  conf_server.begin();
  configuration_server = true;
}

bool wifi_configure_server_handler() {
  WiFiClient client = conf_server.available();

  if (client) {
    // Serial.println("New client");
    String currentLine = "";
    while (client.connected()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          client.println(HTMLPart1);
          update_network_scan();
          String temp = networks;
          while(temp.indexOf('~') != -1) {
            int index = temp.indexOf('~');
            String ssidName = temp.substring(0, index);
            client.print("<option value=\"");
            client.print(ssidName);
            client.print("\">");
            client.print(ssidName);
            client.println("</option>");
            temp = temp.substring(index + 1);
          }
          client.println(HTMLPart2);

          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      } else {
        // Serial.println(currentLine);
        int ssidIndex = currentLine.indexOf("?ssid=");
        int passIndex = currentLine.indexOf("&password=");
        if (ssidIndex != -1 && passIndex != -1) {
          Serial.println("Storing ssid and password to EEPROM");
          String ssid = currentLine.substring(ssidIndex + 6, passIndex);
          ssid.replace("%20", " ");
          // Serial.print("ssid: ");
          // Serial.print(ssid);
          // Serial.print(" length: ");
          // Serial.println(ssid.length());
          String pass = currentLine.substring(passIndex + 10, currentLine.indexOf('~'));
          pass.replace("%20", " ");
          // Serial.print("pass: ");
          // Serial.print(pass);
          // Serial.print(" length: ");
          // Serial.println(pass.length());
          EEPROM.writeUInt(ssidAddr, ssid.length());
          EEPROM.writeString(ssidAddr + 4, ssid);
          EEPROM.writeUInt(passAddr, pass.length());
          EEPROM.writeString(passAddr + 4, pass);
          client.println("Connecting to the given network...\nThis could take up to 30 seconds.\nThe previous page should change if a connection is established.\n\nShutting down the access point.");
          delay(5000);
          WiFi.softAPdisconnect(true);
          return true;
        }
      }
    }
  }
  return false;
}

void wifi_connect() {
  uint ssidLength = EEPROM.readUInt(ssidAddr);
  uint passLength = EEPROM.readUInt(passAddr);
  // Serial.print("configured ssid name length: ");
  // Serial.println(ssidLength);
  // Serial.print("configured password length: ");
  // Serial.println(passLength);
  if (ssidLength != 255 && passLength != 255) {
    char ssid[ssidLength]; 
    char pass[passLength];
    EEPROM.readString(ssidAddr + 4, ssid, ssidLength);
    EEPROM.readString(passAddr + 4, pass, passLength);
    // Serial.println(ssid);
    // Serial.println(pass);

    Serial.print("Connecting");
    int status = WL_IDLE_STATUS;
    unsigned long maxTime = millis() + connection_timeout;
    WiFi.begin(ssid, pass);
    while ((status != WL_CONNECTED || status != WL_CONNECT_FAILED) && millis() < maxTime) {
      status = WiFi.status();
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to connect to network");
    } else {
      connection_established = true;
      Serial.println("Connected successfully ");
      if (configuration_server) {
        Serial.print("Be sure to go to the new ip address: ");
        Serial.println(WiFi.localIP());
      }
      if(!MDNS.begin(mdns_name)) {
        Serial.println("Something went wrong with the mdns responder");
        while (true);
      }
    }
  } else{
    Serial.println("configuration failed or no configuration found");
    EEPROM.writeUInt(ssidAddr, 4294967295);
    EEPROM.writeUInt(passAddr, 4294967295);
  }
}

AsyncWebServer main_server(80);

const char main_html[] = 
R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>

    <!-- main toggle switch to turn on -->
    <label class="switch">
        <input type="checkbox" id="toggle" name="toggle" onchange="update(this)" %PLACEHOLDER_POWERED%>
        <!-- <span class="slider round"></span><br> -->
    </label>

    <!-- slider for brightness and temperature -->
    <div id="slidecontainer" class="slidecontainer">
        <label for="brightness">Brightness:</label>
        <input type="range" min="0" max="255" value="%PLACEHOLDER_BRIGHTNESS%" class="slider" id="brightness" name="brightness_slider"
            onchange="update(this)">
        <input type="number" id="brightnessValue" name="brightness_output" value="%PLACEHOLDER_BRIGHTNESS%" min="0" max="255"><br>

        <label for="temperature">Temperature:</label>
        <input type="range" min="0" max="255" value="%PLACEHOLDER_TEMPERATURE%" class="slider" id="temperature" name="temperature_slider"
            onchange="update(this)">
        <input type="number" id="temperatureValue" name="temperature_output" value="%PLACEHOLDER_TEMPERATURE%" min="0" max="255">
    </div>

    <script>
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
            if (this.value > 255) {
                this.value = 255;
            } else if (this.value < 0) {
                this.value = 0;
            }
            brightness_slider.value = this.value;
        }
        temperature_slider.oninput = function () {
            temperature_output.value = this.value;
        }
        temperature_output.oninput = function () {
            if (this.value > 255) {
                this.value = 255;
            } else if (this.value < 0) {
                this.value = 0;
            }
            temperature_slider.value = this.value;
        }

        function update(element) {
            var xhr = new XMLHttpRequest();
            if (element.id == "toggle") {
              console.log("toggled");
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
</body>
</html>
)=====";

String processor(const String& var) {
  // Serial.println(var);
  if (var == "PLACEHOLDER_POWERED") {
    return (lampState.powered) ? String("checked") : String();
  } else if(var == "PLACEHOLDER_BRIGHTNESS") {
    return String(lampState.brightness);
  } else if(var == "PLACEHOLDER_TEMPERATURE") {
    return String(lampState.temperature);
  }
  return String();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not Found");
}

void setup_main_server() {
  main_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", main_html, processor);
  });
  main_server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Serial.println("change made!");
    String outputType;
    int state;
    if (request->hasParam("output") && request->hasParam("state")) {
      outputType = request->getParam("output")->value();
      state = request->getParam("state")->value().toInt();
    }
    if (outputType == "powered") {
      lampState.powered = state;
      // Serial.print("Changed lamp state powered to: ");
      // Serial.println(lampState.powered);
    } else if (outputType == "brightness") {
      lampState.brightness = state;
      // Serial.print("Changed lamp state brightness to: ");
      // Serial.println(lampState.brightness);
    } else if (outputType == "temperature") {
      lampState.temperature = state;
      // Serial.print("Changed lamp state temperature to: ");
      // Serial.println(lampState.temperature);
    }
    request->send(200, "text/html", "request sent!");
  });
  main_server.onNotFound(notFound);
  main_server.begin();
}
