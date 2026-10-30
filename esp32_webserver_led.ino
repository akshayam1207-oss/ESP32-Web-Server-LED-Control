#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);


const int ledPin = LED_BUILTIN;

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 LED Control</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        background: #f2f4f8;
        padding: 40px 10px;
      }
      .card {
        background: white;
        max-width: 400px;
        margin: auto;
        padding: 25px;
        border-radius: 15px;
      }
      button {
        padding: 15px 30px;
        margin: 10px;
        border: none;
        border-radius: 8px;
        color: white;
        font-size: 18px;
        cursor: pointer;
      }
      .on { background: #28a745; }
      .off { background: #dc3545; }
    </style>
  </head>
  <body>
    <div class="card">
      <h1>ESP32 Web Server</h1>
      <p>Control the built-in LED using Wi-Fi.</p>
      <a href="/on"><button class="on">TURN ON</button></a>
      <a href="/off"><button class="off">TURN OFF</button></a>
    </div>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleLEDOn() {
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLEDOff() {
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("Open this address in your browser: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
