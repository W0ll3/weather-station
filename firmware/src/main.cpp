#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiHelper.h>   // deine eigene Bibliothek
#include <MillisTimer.h>  // deine eigene Bibliothek
#include <DHT.h>
#include "DHTSensor.h"    // <--- NEU

// WLAN-Zugang
const char* SSID     = "XYZ";
const char* PASSWORD = "XYZ";

// Rohwerte (ADC)
struct rawDataSensors { float tempRaw; float humRaw; };
rawDataSensors data = {NAN, NAN};

// Timer
MillisTimer printIntervall(5000);

// --- Sensor(e) ---
// KY-015 (DHT11) am Digitalpin D2
DHTSensor sensor_hum_temp(2, DHT11);

// --- Vorwärtsdeklaration ---
void readSensors();
void reportMessage();

void setup() {
  Serial.begin(115200);
  sensor_hum_temp.begin();        // <--- WICHTIG: Hardware initialisieren
  connectWiFi(SSID, PASSWORD);    // aus deiner WiFiHelper-Lib
}

void loop() {
  maintainWiFi(SSID, PASSWORD);   // Reconnect-Logik aus deiner Lib
  readSensors();
  reportMessage();
}

// Sensoren auslesen
void readSensors() {
  sensor_hum_temp.update();                       // misst Temp & Hum und puffert intern
  data.tempRaw = sensor_hum_temp.getTemperature();
  data.humRaw  = sensor_hum_temp.getHumidity();    
}

// Bericht über die Sensorwerte -> alle 5 Sekunden
void reportMessage() {
  if (printIntervall.ready()) {
    Serial.print("RAW T/H: ");
    Serial.print(data.tempRaw); Serial.print(" / ");
    Serial.print(data.humRaw);  Serial.println(" %");
  }
}
