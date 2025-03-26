#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h> // For parsing JSON

// WiFi credentials
const char* ssid = "Habbo Hotel";
const char* password = "Qu!ck!3W33";

WebServer server(80); // Web server on port 80

int cwBrightness = 50;
int wwBrightness = 50;
int lastCwBrightness = cwBrightness;
int lastWwBrightness = wwBrightness;
int lastUpdate = millis(); // used for debug
bool built_in_led = true; // used for debug
bool showcw = false;
bool showww = !showcw;


const int cwPin = 0;
const int wwPin = 1;
const int onboardLED = 10; // used for debug
const int freq = 1000;
const int resolution = 8;

void handleRoot() {
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

void handleUpdate() {
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

    int wwValue = doc["wwValue"];
    int cwValue = doc["cwValue"];
    Serial.print("Slider Values: ");
    Serial.print("cold white: ");
    Serial.print(wwValue);
    Serial.print("  warm white: ");
    Serial.println(cwValue);
    cwBrightness = map(cwValue, 0, 100, 0, 255);
    wwBrightness = map(wwValue, 0, 100, 0, 255);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void updateLedValues() {
  int millisBetween = 500;
  bool update = lastWwBrightness != wwBrightness || lastCwBrightness != cwBrightness;
  if (built_in_led) {
    if (millis() > lastUpdate + millisBetween) {
      lastUpdate = millis();
      showww = !showww;
      showcw = !showcw;
      update = true;
    }
    if (showww) {
      if (update) {
        Serial.print("Displaying on the on board led: Warm white brightness: ");
        Serial.println(wwBrightness);
        ledcWrite(onboardLED, wwBrightness);
        lastWwBrightness = wwBrightness;
      }
    } else if (showcw) {
      if (update) {
        Serial.print("Displaying on the on board led: Cool white brightness: ");
        Serial.println(cwBrightness);
        ledcWrite(onboardLED, cwBrightness);
        lastCwBrightness = cwBrightness;
      }
    } else {
      Serial.println("ERROR: no brightness is displayed with the on board led");
      Serial.print("showcw value: ");
      Serial.println(showcw);
      Serial.print("  showww value: ");
      Serial.print(showww);
    }
  }
  ledcWrite(cwPin, cwBrightness);
  ledcWrite(wwPin, wwBrightness);
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Starting up...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/update_values", HTTP_POST, handleUpdate);

  ledcAttach(cwPin, freq, resolution);
  ledcAttach(wwPin, freq, resolution);
  ledcAttach(onboardLED, freq, resolution);

  server.begin();
  Serial.println("Web server started");
}



void loop() {
  server.handleClient();
  updateLedValues();
}