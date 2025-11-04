#include <Arduino.h>  //<<< PlatformIO
#include "WiFiHelper.h" //<<< Lib - Verbindung zum WiFi
#include "MillisTimer.h" //<<< Lib - Timer
#include "DHTSensor.h" //<<< Klasse - für Sensor DHT11
#include "UbidotsRaw.h" //<<< HTTP verbindung 

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
MillisTimer printIntervall(5000);     // <<< 
MillisTimer uplinkIntervall(10000);    // <<< Alle 10s zu Ubidots senden

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

// <<< NEU: HTTP POST an Ubidots -> Periodisch -> Daten prüfen -> Body bauen ->  
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

  bool uploadSuccess = UbidotsRaw::postDevice(net, UBIDOTS_HOST, DEVICE_LABEL, UBIDOTS_TOKEN, body);
  if (uploadSuccess) {
    Serial.println("[Ubidots] Upload erfolgreich");
    } 
    else {
    Serial.println("[Ubidots] Upload fehlgeschlagen");
    }
}
