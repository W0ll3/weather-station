#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiHelper.h>
#include <MillisTimer.h>
#include <DHT.h>
#include "DHTSensor.h"
#include <UbidotsRaw.h>               // <<< NEU

// WLAN-Zugang
const char* SSID     = "XYZ";
const char* PASSWORD = "XYZ";

// Ubidots
static const char* UBIDOTS_HOST  = "industrial.api.ubidots.com"; // STEM: "things.ubidots.com"
static const char* UBIDOTS_TOKEN = "BBFF-abc";                   // <<< DEIN Token eintragen
static const char* DEVICE_LABEL  = "wetterstation";              // <<< klein/0-9/_/-

// Rohwerte (ADC)
struct rawDataSensors { float tempRaw; float humRaw; };
rawDataSensors data = {NAN, NAN};

// Timer
MillisTimer printIntervall(5000);
MillisTimer uplinkIntervall(10000);    // <<< NEU: alle 10s zu Ubidots senden

// Sensor(e)
// KY-015 (DHT11) am Digitalpin D2
DHTSensor sensor_hum_temp(2, DHT11);

// Vorwärtsdeklaration
void readSensors();
void reportMessage();
void sendUbidots();                    // <<< NEU

void setup() {
  Serial.begin(115200);
  sensor_hum_temp.begin();
  connectWiFi(SSID, PASSWORD);
}

void loop() {
  maintainWiFi(SSID, PASSWORD);
  readSensors();
  reportMessage();
  sendUbidots();                       // <<< NEU
}

// Sensoren auslesen
void readSensors() {
  sensor_hum_temp.update();
  data.tempRaw = sensor_hum_temp.getTemperature();
  data.humRaw  = sensor_hum_temp.getHumidity();
}

// Bericht seriell
void reportMessage() {
  if (printIntervall.ready()) {
    Serial.print("RAW T/H: ");
    Serial.print(data.tempRaw); Serial.print(" / ");
    Serial.print(data.humRaw);  Serial.println(" %");
  }
}

// <<< NEU: HTTP POST an Ubidots
void sendUbidots() {
  if (!uplinkIntervall.ready()) return;
  if (isnan(data.tempRaw) || isnan(data.humRaw)) {
    Serial.println("[Ubidots] Werte noch nicht gültig");
    return;
  }

  // JSON bauen
  String body = UbidotsRaw::makeBodyTempHum(data.tempRaw, data.humRaw);

  // verbundenen Client erzeugen (WiFiS3 stellt den TCP-Client)
  WiFiClient net; // lokal, pro POST neue Verbindung (Connection: close)

  bool ok = UbidotsRaw::postDevice(net, UBIDOTS_HOST, DEVICE_LABEL, UBIDOTS_TOKEN, body);
  Serial.println(ok ? "[Ubidots] OK" : "[Ubidots] FEHLER");
}
