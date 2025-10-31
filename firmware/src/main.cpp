#include <Arduino.h>
#include <WiFiS3.h>

// WLAN-Zugang
const char* SSID     = "XYZ"; //Name des Netzwerk
const char* PASSWORD = "XYZ"; //Passwort des Netzwerks

// Pin-Zuordnung (klar und zentral)
const uint8_t PIN_TEMP  = A0;
const uint8_t PIN_HUM   = A1;
const uint8_t PIN_LUX   = A2;
const uint8_t PIN_PRESS = A3;

// Rohwerte (ADC)
int      tempRaw  = 0;
uint16_t humRaw   = 0;   // 0..1023 (später skalieren auf %)
uint16_t luxRaw   = 0;   // 0..65535 wäre genug, ADC liefert 0..1023
int      pressRaw = 0;

// Timer für Statusausgaben
const unsigned long INTERVAL_MS = 2500;
unsigned long lastStatus = 0;

// Vorwärtsdeklarationen
void connectWiFi();
void readSensors();

void setup() {
  Serial.begin(115200);
  delay(200);
  connectWiFi();
}

void loop() {
  readSensors();

  // Platzhalter: hier später Umrechnung + Senden an Ubidots
  // Beispiel-Debug:
  // Serial.print("RAW T/H/L/Pr: ");
  // Serial.print(tempRaw); Serial.print('/');
  // Serial.print(humRaw);  Serial.print('/');
  // Serial.print(luxRaw);  Serial.print('/');
  // Serial.println(pressRaw);

  delay(200); // nur zum Ruhigstellen der Demo
}

void readSensors() {
  tempRaw  = analogRead(PIN_TEMP);
  humRaw   = analogRead(PIN_HUM);
  luxRaw   = analogRead(PIN_LUX);
  pressRaw = analogRead(PIN_PRESS);
}

void connectWiFi() {
  Serial.print("Verbinde mit WLAN: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastStatus >= INTERVAL_MS) {
      lastStatus = millis();
      Serial.println("... WLAN noch nicht verbunden");
    }
    delay(200);
  }

  Serial.println("WLAN verbunden.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

