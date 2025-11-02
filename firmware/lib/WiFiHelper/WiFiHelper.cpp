#include "WiFiHelper.h"
#include "MillisTimer.h"

static unsigned long lastCheck = 0;
MillisTimer loadingMS(200);
MillisTimer intervallMS(2000);


void connectWiFi(const char* ssid, const char* password) {
  Serial.print("Verbinde mit WLAN: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (loadingMS.ready()) {
    Serial.print(".");
    }
  }

  Serial.println("\nWLAN verbunden!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void maintainWiFi(const char* ssid, const char* password) {
  if (intervallMS.ready()) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WLAN getrennt – versuche Neuverbindung...");
      WiFi.begin(ssid, password);
    }
  }
}
