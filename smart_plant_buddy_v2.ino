#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>

// --- CONFIG ---
const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";

// --- PINS ---
#define MOISTURE_PIN 34   // Analog pin for soil moisture
#define DHT_PIN 25        // DHT sensor data pin
#define DHT_TYPE DHT22    // Change to DHT11 if needed
#define LED_PIN 14
#define NUMPIXELS 1

// --- OBJECTS ---
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHT_PIN, DHT_TYPE);

// --- VARIABLES ---
int moistureValue;
int dryThreshold = 1500; // Adjust this after calibration
float temperature;
float humidity;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.show();
  dht.begin();
  connectWiFi();

  Serial.println("Smart Plant Buddy initialized!");
}

void loop() {
  // --- Read sensors ---
  moistureValue = analogRead(MOISTURE_PIN);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // --- Check readings ---
  Serial.print("Soil Moisture: ");
  Serial.print(moistureValue);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // --- LED status logic ---
  if (moistureValue < dryThreshold) {
    setLEDColor(255, 0, 0);   // Red = dry
  } else {
    setLEDColor(0, 255, 0);   // Green = happy
  }

  delay(3000); // 3 seconds between readings
}

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected!");
  } else {
    Serial.println(" Failed to connect (continuing offline mode).");
  }
}

void setLEDColor(int r, int g, int b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}
