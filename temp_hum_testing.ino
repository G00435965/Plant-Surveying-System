#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include "ThingSpeak.h"
#include <DFRobot_DHT11.h>

#define SECRET_SSID "eir75114808"
#define SECRET_PASS "4eUFQDa9qF"
#define SECRET_CH_ID 2773493
#define SECRET_WRITE_APIKEY "1JK87NRUHJDT7ZM6"

#define MOISTURE_PIN 34
#define THRESHOLD_VALUE 300
#define MOTOR_PIN 25
#define DHT11_PIN 27
#define LED_PIN 26

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

DFRobot_DHT11 DHT;
WiFiClient client;
WebServer server(80);

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

float tempC = 0;
float humidity = 0;
int moistureLevel = 0;
String myStatus = "";
bool ledState = false;

String getSensorReadings() {
  DHT.read(DHT11_PIN);
  tempC = DHT.temperature;
  humidity = DHT.humidity;
  moistureLevel = analogRead(MOISTURE_PIN);

  // Check if DHT readings are valid
  if (isnan(tempC) || isnan(humidity)) {
    return "Failed to read from DHT sensor!";
  }

  String readings = "Temperature: " + String(tempC, 1) + " °C<br>";
  readings += "Humidity: " + String(humidity, 1) + " %<br>";
  readings += "Soil Moisture: " + String(moistureLevel) + "<br>";
  return readings;
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void handleTemperature() {
  DHT.read(DHT11_PIN);
  server.send(200, "text/plain", String(DHT.temperature, 1));
}

void handleRoot() {
  String message = homePagePart1 + getSensorReadings();
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  message += "Arguments: " + String(server.args()) + "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  ThingSpeak.begin(client);

  server.on("/", handleRoot);
  server.on("/temperature", handleTemperature);
  server.on("/toggleLED", toggleLED);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

  DHT.read(DHT11_PIN);
  tempC = DHT.temperature;
  humidity = DHT.humidity;
  moistureLevel = analogRead(MOISTURE_PIN);

  // Debug output to Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(tempC);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Soil Moisture: ");
  Serial.println(moistureLevel);

  if (!isnan(tempC) && !isnan(humidity)) {
    if (tempC > 25 && humidity > 60)
      myStatus = "Temperature and humidity too high.";
    else if (tempC > 25)
      myStatus = "Temperature is too high.";
    else if (humidity > 60)
      myStatus = "Humidity is too high.";
    else
      myStatus = "Temperature and humidity are fine.";
  }

  if (moistureLevel < THRESHOLD_VALUE) {
    analogWrite(MOTOR_PIN, 50);
    delay(2000);
    analogWrite(MOTOR_PIN, 100);
    delay(4000);
    digitalWrite(MOTOR_PIN, HIGH);
    delay(2000);
    digitalWrite(MOTOR_PIN, LOW);
    delay(2000);
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(4, moistureLevel);
  ThingSpeak.setStatus(myStatus);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("ThingSpeak update failed: " + String(x));
  }

  delay(20000);
}
