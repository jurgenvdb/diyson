#include "WiFiType.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "html.h"

LampState lamp_state = {false, 0, 2700};
WiFiServer conf_server(configuration_server_port);
AsyncWebServer main_server(main_server_port);

unsigned long last_scan_update = -scan_interval;
bool configuration_server = false;
bool connection_established = false;

void wifi_configure(String ssid, String password) {
  EEPROM.writeUInt(ssid_length_addr, ssid.length());
  EEPROM.writeString(ssid_addr, ssid);
  EEPROM.writeUInt(pass_length_addr, password.length());
  EEPROM.writeString(pass_addr, password);
}

bool wifi_is_configured() {
  uint ssidLength = EEPROM.readUInt(ssid_length_addr);
  uint passLength = EEPROM.readUInt(pass_length_addr);
  if (ssidLength < ssid_length && passLength < pass_length) {
    return true;
  }
  return false;
}

void update_network_scan(String &networks) {
  if (millis() > last_scan_update + scan_interval) {
    last_scan_update = millis();
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
  WiFi.softAP(default_ssid);
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
          client.println("Loading...");
          String networks = "";
          update_network_scan(networks);
          String output = config_html[0];
          while(networks.indexOf('~') != -1) {
            int index = networks.indexOf('~');
            String ssidName = networks.substring(0, index);
            output += "<option value=\"" + ssidName + "\">" + ssidName + "</option>";
            networks = networks.substring(index + 1);
          }
          output += config_html[1];
          client.flush();
          client.println(output);
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
          EEPROM.writeUInt(ssid_length_addr, ssid.length());
          EEPROM.writeString(ssid_addr, ssid);
          EEPROM.writeUInt(pass_length_addr, pass.length());
          EEPROM.writeString(pass_addr, pass);
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
  uint ssidLength = EEPROM.readUInt(ssid_length_addr);
  uint passLength = EEPROM.readUInt(pass_length_addr);
  // Serial.print("configured ssid name length: ");
  // Serial.println(ssidLength);
  // Serial.print("configured password length: ");
  // Serial.println(passLength);
  if (ssidLength != ssid_length && passLength != pass_length) {
    char ssid[ssidLength]; 
    char pass[passLength];
    EEPROM.readString(ssid_addr, ssid, ssidLength);
    EEPROM.readString(pass_addr, pass, passLength);
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
    EEPROM.writeUInt(ssid_length_addr, uint_size);
    EEPROM.writeUInt(pass_length_addr, uint_size);
  }
}

String processor(const String& var) {
  // Serial.println(var);
  if (var == "PLACEHOLDER_POWERED") {
    return (lamp_state.powered) ? String("checked") : String();
  } else if(var == "PLACEHOLDER_BRIGHTNESS") {
    return String(lamp_state.brightness);
  } else if(var == "PLACEHOLDER_TEMPERATURE") {
    return String(lamp_state.temperature);
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
      lamp_state.powered = state;
      // Serial.print("Changed lamp state powered to: ");
      // Serial.println(lamp_state.powered);
    } else if (outputType == "brightness") {
      lamp_state.brightness = state;
      // Serial.print("Changed lamp state brightness to: ");
      // Serial.println(lamp_state.brightness);
    } else if (outputType == "temperature") {
      lamp_state.temperature = state;
      // Serial.print("Changed lamp state temperature to: ");
      // Serial.println(lamp_state.temperature);
    }
    request->send(200, "text/html", "request sent!");
  });
  main_server.onNotFound(notFound);
  main_server.begin();
}
