#include "WiFiType.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define ssidAddr 0
#define passAddr 250

#define defaultSsid String("DIYson lamp project")
#define connection_timeout 30000

WiFiServer server(80);

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
  Serial.print("configured ssid name length: ");
  Serial.println(ssidLength);
  Serial.print("configured password length: ");
  Serial.println(passLength);
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
  server.begin();
  configuration_server = true;
}

bool wifi_configure_server_handler() {
  WiFiClient client = server.available();

  if (client) {
    // Serial.println("New client");
    String currentLine = "";
    while (client.connected()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          if (!connection_established) {
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
          } else {
            client.println("connection established, This access point should dissapear automatically.\nThe new web-interface is located on the same network as the configured one, under the following ip address: " + WiFi.localIP());
            delay(1000);
            WiFi.softAPdisconnect(true);
          }

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
          client.println("Connecting to the given network...\nThis could take up to 30 seconds.<br>The previous page should change if a connection is established.");
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
  Serial.print("configured ssid name length: ");
  Serial.println(ssidLength);
  Serial.print("configured password length: ");
  Serial.println(passLength);
  if (ssidLength != 255 && passLength != 255) {
    char ssid[ssidLength]; 
    char pass[passLength];
    EEPROM.readString(ssidAddr + 4, ssid, ssidLength);
    EEPROM.readString(passAddr + 4, pass, passLength);
    Serial.println(ssid);
    Serial.println(pass);

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
    }
  } else{
    Serial.println("configuration failed or no configuration found");
  }
}

void setup_main_server() {
  // TO-DO
  // https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
  // https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
}
