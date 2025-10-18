#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// --- CONFIG ---
const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";

// --- PINS ---
#define MOISTURE_PIN 34
#define DHT_PIN 25
#define LED_PIN 14
#define NUMPIXELS 1

// --- LED SETUP ---
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// --- VARIABLES ---
int moistureValue;
int threshold = 1500; // adjust later when testing real sensor

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.show(); 
  connectWiFi();
}

void loop() {
  // Fake moisture value if you don't have the sensor yet
  moistureValue = random(1000, 3000); 

  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  if (moistureValue < threshold) {
    setLEDColor(255, 0, 0);  // Red = dry
  } else {
    setLEDColor(0, 255, 0);  // Green = happy
  }

  delay(2000);
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

