#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiHelper.h>  // <--- deine eigene Bibliothek
#include <MillisTimer.h>

// WLAN-Zugang
const char* SSID     = "XYZ"; // Name des Netzwerks
const char* PASSWORD = "XYZ"; // Passwort des Netzwerks

// Pin-Zuordnung
const uint8_t PIN_TEMP  = A0;
const uint8_t PIN_HUM   = A1;
const uint8_t PIN_LUX   = A2;
const uint8_t PIN_PRESS = A3;

// Rohwerte (ADC)
struct rawDataSensors{ int tempRaw; uint16_t humRaw; uint16_t luxRaw; int pressRaw; };

rawDataSensors data;

// Timer

MillisTimer printIntervall(5000);

// --- Vorwärtsdeklaration ---
void readSensors();
void reportMessage();

void setup() {
  Serial.begin(115200);
  connectWiFi(SSID, PASSWORD);   // <-- aus deiner Library
}

void loop() {
  maintainWiFi(SSID, PASSWORD);  // <-- sorgt für Reconnect, wenn WLAN weg ist
  readSensors();
  reportMessage();
}

// Sensoren auslesen
void readSensors() {
  data.tempRaw  = analogRead(PIN_TEMP);
  data.humRaw   = analogRead(PIN_HUM);
  data.luxRaw   = analogRead(PIN_LUX);
  data.pressRaw = analogRead(PIN_PRESS);
}

// Bericht über die Sensorwerte -> Alle 5 Sekunden
void reportMessage() {
if (printIntervall.ready()) {
  Serial.print("RAW T/H/L/Pr: ");
  Serial.print(data.tempRaw); Serial.print('/');
  Serial.print(data.humRaw);  Serial.print('/');
  Serial.print(data.luxRaw);  Serial.print('/');
  Serial.println(data.pressRaw);
  }
}

