#include "WiFiHelper.h"
#include "MillisTimer.h"

MillisTimer loadingMS(500);
MillisTimer timeoutMS(10000);
MillisTimer retryMS(5000);


void connectWiFi(const char* ssid, const char* password) {
  
  Serial.print("Verbinde mit WLAN: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (loadingMS.ready()) Serial.print(".");
    if (timeoutMS.ready()) {
      Serial.println("\nKeine Verbindung – fahre trotzdem fort.");
    break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWLAN verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WLAN nicht verbunden. Offline-Modus aktiv.");
  }
}

void maintainWiFi(const char* ssid, const char* password) {
  if (retryMS.ready()) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WLAN getrennt – versuche Neuverbindung...");
      WiFi.begin(ssid, password);
    }
  }
}
