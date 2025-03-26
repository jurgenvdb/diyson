#include "webserver.h"
#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

Webserver::Webserver(const char* ssid, const char* password) {
  server = server(80);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IP = WiFi.localIP();
  server.on("/", handleRoot);
  server.on("/update_values", HTTP_POST, handleUpdate);
  server.begin();
}

Webserver::getCoolWhiteDuty() {
  return coolWhiteDuty;
}

Webserver::getWarmWhiteDuty() {
  return warmWhiteDuty;
}

Webserver::handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Slider Control</title></head><body>";
  html += "<h2>Warm white slider</h2>";
  html += "<input type='range' id='wwSlider' min='0' max='100' value='50'><br>";
  html += "<p>Warm white value: <span id='wwSliderValue'>50</span></p>";
  html += "<h2>Cold white slider</h2>";
  html += "<input type='range' id='cwSlider' min='0' max='100' value='50'><br>";
  html += "<p>Cold white value: <span id='cwSliderValue'>50</span></p>";
  html += "<script>";
  html += "var wwSlider = document.getElementById('wwSlider');";
  html += "var wwSliderOutput = document.getElementById('wwSliderValue');";
  html += "var cwSlider = document.getElementById('cwSlider');";
  html += "var cwSliderOutput = document.getElementById('cwSliderValue');";
  html += "wwSliderOutput.innerHTML = wwSlider.value;";
  html += "cwSliderOutput.innerHTML = cwSlider.value;";
  html += "function postValues() {";
  html += "  wwSliderOutput.innerHTML = wwSlider.value;";
  html += "  cwSliderOutput.innerHTML = cwSlider.value;";
  html += "  fetch('/update_values', {";
  html += "    method: 'POST',";
  html += "    header: {";
  html += "      'Content-type': 'application/json',";
  html += "    },";
  html += "    body: JSON.stringify({";
  html += "      wwValue: wwSlider.value,";
  html += "      cwValue: cwSlider.value";
  html += "    }),";
  html += "  }).then(response => {";
  html += "    if (!response.ok) {";
  html += "      console.error('Error sending data');";
  html += "    }";
  html += "  }).catch(error => {";
  html += "    console.error('Fetch error:', error);";
  html += "  });";
  html += "}";
  html += "wwSlider.oninput = postValues;";
  html += "cwSlider.oninput = postValues;";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

Webserver::handleUpdate() {
  if (server.method() == HTTP_POST) {
    if (server.hasArg("plain") == false || server.arg("plain").length() == 0) {
      server.send(400, "text/plain", "Bad Request: Missing JSON data");
      return;
    }

    String jsonString = server.arg("plain");

    DynamicJsonDocument doc(1024); // Adjust size as needed
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
      server.send(400, "text/plain", "Bad Request: Invalid JSON");
      return;
    }

    coolWhiteDuty = map(doc["cwValue"], 0, 100, 255, 0);
    warmWhiteDuty = map(doc["wwValue"], 0, 100, 255, 0);
    Serial.print("Slider Values: ");
    Serial.print("cold white: ");
    Serial.print(coolWhiteDuty);
    Serial.print("  warm white: ");
    Serial.println(warmWhiteDuty);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

Webserver::getIP() {
  return IP;
}