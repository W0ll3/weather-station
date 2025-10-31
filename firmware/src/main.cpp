#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiHelper.h>  // <--- deine eigene Bibliothek

// WLAN-Zugang
const char* SSID     = "XYZ"; // Name des Netzwerks
const char* PASSWORD = "XYZ"; // Passwort des Netzwerks

// Pin-Zuordnung
const uint8_t PIN_TEMP  = A0;
const uint8_t PIN_HUM   = A1;
const uint8_t PIN_LUX   = A2;
const uint8_t PIN_PRESS = A3;

// Rohwerte (ADC)
int      tempRaw  = 0;
uint16_t humRaw   = 0;
uint16_t luxRaw   = 0;
int      pressRaw = 0;

// --- Vorwärtsdeklaration ---
void readSensors();

void setup() {
  Serial.begin(115200);
  connectWiFi(SSID, PASSWORD);   // <-- aus deiner Library
}

void loop() {
  maintainWiFi(SSID, PASSWORD);  // <-- sorgt für Reconnect, wenn WLAN weg ist
  readSensors();

  // Beispiel-Debug-Ausgabe
  Serial.print("RAW T/H/L/Pr: ");
  Serial.print(tempRaw); Serial.print('/');
  Serial.print(humRaw);  Serial.print('/');
  Serial.print(luxRaw);  Serial.print('/');
  Serial.println(pressRaw);

  delay(200); // später ersetzen durch millis()
}

// Sensoren auslesen
void readSensors() {
  tempRaw  = analogRead(PIN_TEMP);
  humRaw   = analogRead(PIN_HUM);
  luxRaw   = analogRead(PIN_LUX);
  pressRaw = analogRead(PIN_PRESS);
}
