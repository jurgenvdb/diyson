/*
* driver pwm pin connected to collector
* driver ground and esp ground connected to emitter
* pwm signal connected to base trough a 530 ohm resistor
* inverts pwm signal to brightness of led driver
* meaning the higher the duty cycle, the dimmer the led
* 
*/



#include <WiFi.h>

const char* html = 
R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Small test for web and driver</title>
</head>
<body>
    <form>
        <input type="number" min="0", max="255" value="0">
        <input type="submit">
    </form>
</body>
<script>
document.querySelector('form').addEventListener('submit', function(event) {
    event.preventDefault();
    var inputValue = document.querySelector('input[type="number"]').value;
    var url = 'brightness/' + inputValue + '/';
    // make the browser go to the url
    window.location.href += url
});
</script>
</html>
)=====";


const char* ssid = "Habbo Hotel";
const char* password = "Qu!ck!3W33";

const int ledPin = 10;
const int driverPin = 0;
const int freq = 1000;
const int resolution = 8;

String header;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println(ledcAttach(ledPin, freq, resolution));
  Serial.println(ledcAttach(driverPin, freq, resolution));

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentline = "";
    while(client.available()) {
      if (client.available()){
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentline.length() == 0){
            client.println(html);

            if (header.indexOf("/brightness/") >= 0) {
              int beginIndex = header.indexOf("/brightness/") + 12;
              int endIndex = header.substring(beginIndex).indexOf("/");
              // Serial.println(beginIndex);
              // Serial.println(endIndex);
              Serial.println(header.substring(beginIndex, beginIndex + endIndex));
              
              int brightness = header.substring(beginIndex, beginIndex + endIndex).toInt();
              ledcWrite(ledPin, brightness);
              ledcWrite(driverPin, brightness);
            }
          } else {
            currentline = "";
          }
        } else if (c != '\r') {
          currentline += c;
        }
      }
      
    }
  }
  header = "";
  client.stop();
}