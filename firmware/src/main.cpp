#include <Arduino.h>  //<<< PlatformIO
#include "WiFiHelper.h" //<<< Lib - Verbindung zum WiFi
#include "MillisTimer.h" //<<< Lib - Timer
#include "DHTSensor.h" //<<< Klasse - für Sensor DHT11
#include "UbidotsRaw.h" //<<< HTTP verbindung 
#include "secrets.h"   //<<< Geheime Daten

// Vorwärtsdeklarationen
void reportMessage();

// Setup und Loop
void setup() {
  Serial.begin(115200);
  delay(5000); // Warte auf Serielle;
  sensor_hum_temp.begin();
  connectWiFi(WIFI_SSID, WIFI_PASS);
  printIntervall.reset();
}

void loop() {
  readSensors();  // Sensoren auslesen
  reportMessage(); // Bericht seriell
  maintainWiFi(WIFI_SSID, WIFI_PASS); // WiFi Verbindung aufrechterhalten
  sendUbidots(); // Daten zu Ubidots senden
}


// Bericht seriell
void reportMessage() {
  if (isnan(data.tempRaw) || isnan(data.humRaw)) return;
  if (printIntervall.ready()) {
    Serial.print("RAW T/H: ");
    Serial.print(data.tempRaw); Serial.print(" / ");
    Serial.print(data.humRaw);  Serial.println(" %");
  }
}


