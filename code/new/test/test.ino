#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

class LED_driver {
private:
  int cool_white_pin;
  int warm_white_pin;
  int cool_white_brightness;
  int warm_white_brightness;
  int freq = 1000;
  int res = 8;

public:
  // Define which pin is controlling which LED,
  // frequency and resolution are predefined by default
  LED_driver(int cool_white, int warm_white, int frequency = 1000, int resolution = 8) {
    cool_white_pin = cool_white;
    warm_white_pin = warm_white;
    freq = frequency;
    res = resolution;
    ledcAttach(cool_white_pin, freq, res);
    ledcAttach(warm_white_pin, freq, res);
  }

  // Sets the brightness of the LED (0-255), leave a parameter open to not change that LEDs brightness
  void setBrightness(int brightness_cool_white = -1, int brightness_warm_white = -1) {
    if (brightness_cool_white != -1) {
      setCoolWhiteBrightness(brightness_cool_white);
    }
    if (brightness_warm_white != -1) {
      setWarmWhiteBrightness(brightness_warm_white);
    }
  }

  // Sets the brightness of the cool white LED
  void setCoolWhiteBrightness(int brightness) {
    cool_white_brightness = brightness;
    ledcWrite(cool_white_pin, cool_white_brightness);
  }

  // Sets the brightness of the warm white LED
  void setWarmWhiteBrightness(int brightness) {
    warm_white_brightness = brightness;
    ledcWrite(warm_white_pin, warm_white_brightness);
  }
};

class Webserver {
private:
  WebServer server(80);

  String getHTML(int brightness_cool_white = -1, int brightness_warm_white = -1) {
    if (brightness_cool_white == -1) {
      brightness_cool_white = cool_white_brightness;
    }
    if (brightness_warm_white == -1) {
      brightness_warm_white = warm_white_brightness;
    }
    String html = "<!DOCTYPE html><html><head><title>Slider Control</title></head><body>";
    html += "<h2>Cool white slider</h2>";
    html += "<input type='range' id='cwSlider' min='0' max='255' value='" + String(brightness_cool_white) + "'><br>";
    html += "<p>Cold white value: <span id='cwSliderValue'></span></p>";
    html += "<h2>Warm white slider</h2>";
    html += "<input type='range' id='wwSlider' min='0' max='255' value='" + String(brightness_warm_white) + "'><br>";
    html += "<p>Warm white value: <span id='wwSliderValue'></span></p>";
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
    return html;
  }

  void handleRoot() {
    server.send(200, "text/html", getHTML(cool_white_brightness, warm_white_brightness));
  }

  void handleUpdate() {
    if (server.method() == HTTP_POST) {
      if (server.hasArg("plain") == false || server.arg("plain").length() == 0) {
        server.send(400, "text/plain", "Bad Request: Missing JSON data");
        return;
      }

      String jsonString = server.arg("plain");
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, jsonString);
      if (error) {
        server.send(400, "text/plain", "Bad Request: Invalid JSON");
        return;
      }

      cool_white_brightness = doc["cwValue"];
      warm_white_brightness = doc["wwValue"];
      server.send(200, "text/plain", "OK");
    } else {
      server.send(405, "text/plain", "Method Not Allowed");
    }
  }

public:
  int cool_white_brightness = 0;
  int warm_white_brightness = 0;
  IPAddress IP;

  Webserver(char* ssid, char* password, int initial_cool_white_brightness = 0, int initial_warm_white_brightness = 0) {
    if (!Serial) Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi...");
    }
    IP = WiFi.localIP();
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(IP);

    server().on("/", handleRoot);
    server().on("/update_values", HTTP_POST, handleUpdate);
    server().begin();
    Serial.println("Webserver started");
  }

  void handleClient() {
    server().handleClient();
  }
};



Webserver led_slider_server("Bibliotheek", "Vakantie");
LED_driver led_driver(0, 1);

void setup() {
  // Serial.begin(115200);
  // while(!Serial);
}

void loop() {
  led_slider_server.handleClient();
  Serial.print("Cool white brightness: ");
  Serial.println(led_slider_server.cool_white_brightness);
}
