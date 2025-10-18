#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

// --- CONFIG ---
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_AUTH_TOKEN  "YourAuthToken"

const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";

// --- PINS ---
#define MOISTURE_PIN 34
#define DHT_PIN 25
#define LED_PIN 14
#define NUMPIXELS 1
#define DHTTYPE DHT22 // or DHT11

// --- COMPONENTS ---
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHT_PIN, DHTTYPE);

// --- VARIABLES ---
int moistureValue;
int threshold = 1500; // adjust after calibration
float temperature, humidity;

// --- BLYNK VIRTUAL PINS ---
#define VPIN_TEMP V0
#define VPIN_HUM  V1
#define VPIN_MOISTURE V2

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.show();
  dht.begin();

  connectWiFi();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();

  moistureValue = analogRead(MOISTURE_PIN);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.println("-----");
  Serial.printf("Moisture: %d | Temp: %.1f°C | Humidity: %.1f%%\n", moistureValue, temperature, humidity);

  Blynk.virtualWrite(VPIN_TEMP, temperature);
  Blynk.virtualWrite(VPIN_HUM, humidity);
  Blynk.virtualWrite(VPIN_MOISTURE, moistureValue);

  if (moistureValue < threshold) {
    setLEDColor(255, 0, 0); // dry = red
  } else {
    setLEDColor(0, 255, 0); // good = green
  }

  delay(3000);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void setLEDColor(int r, int g, int b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}
