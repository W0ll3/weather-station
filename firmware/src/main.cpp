#include <Arduino.h>  //<<< PlatformIO
#include "WiFiHelper.h" //<<< Lib - Verbindung zum WiFi
#include "MillisTimer.h" //<<< Lib - Timer
#include "DHTSensor.h" //<<< Klasse - für Sensor DHT11
#include "UbidotsRaw.h" //<<< HTTP verbindung 
#include "secrets.h" //<<< Datei mit SSID und PASSWORD


// Rohwerte (ADC)
struct rawDataSensors { float tempRaw; float humRaw; };
rawDataSensors data = {NAN, NAN};

//timer
  MillisTimer printIntervall(5000);     // <<< Alle 5s seriell ausgeben
  MillisTimer uplinkIntervall(60000);    // <<< Alle 20s zu Ubidots senden

// Sensor(e)
// KY-015 (DHT11) am Digitalpin D2
DHTSensor sensor_hum_temp(2, DHT11);

// Vorwärtsdeklaration
void readSensors();
void reportMessage();
void sendUbidots();

void setup() {
  Serial.begin(115200);
  delay(5000); // Warte auf Serielle;
  sensor_hum_temp.begin();
  connectWiFi(WIFI_SSID, WIFI_PASS);
  printIntervall.reset();
}

void loop() {
  readSensors();
  reportMessage();
  maintainWiFi(WIFI_SSID, WIFI_PASS);
  sendUbidots();
}

// Sensoren auslesen
void readSensors() {
  sensor_hum_temp.update();
  data.tempRaw = sensor_hum_temp.getTemperature();
  data.humRaw  = sensor_hum_temp.getHumidity();
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

// Daten zu Ubidots senden
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

  // Daten senden
  bool uploadSuccess = UbidotsRaw::postDevice(net, UBIDOTS_HOST, DEVICE_LABEL, UBIDOTS_TOKEN, body);
  if (uploadSuccess) {
    Serial.println("[Ubidots] Upload erfolgreich");
    } 
    else {
    Serial.println("[Ubidots] Upload fehlgeschlagen");
    }
}


