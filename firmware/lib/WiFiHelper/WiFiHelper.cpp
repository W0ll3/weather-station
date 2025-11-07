#include "WiFiHelper.h" //<<< Lib - Verbindung zum WiFi
#include "MillisTimer.h" //<<< Lib - Timer

// Timers
  MillisTimer loadingMS(500);
  MillisTimer timeoutMS(10000);
  MillisTimer retryMS(5000);

// Verbindung mit WLAN herstellen
void connectWiFi(const char* ssid, const char* password) {
  
  Serial.print("Verbinde mit WLAN: "); 
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Start WiFi connection

  // Erfolgreich verbunden
  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWLAN verbunden!");
      Serial.print("IP-Adresse: ");
      Serial.println(WiFi.localIP());
    }

  loadingMS.reset(); // Reset loading timer
  timeoutMS.reset(); // Reset timeout timer

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    if (loadingMS.ready()) {
      Serial.print(".");
    }
    if (timeoutMS.ready()) {
      Serial.println("\nKeine Verbindung - fahre trotzdem fort.");
    break;
    }
  }
}  
// WLAN-Verbindung aufrechterhalten
void maintainWiFi(const char* ssid, const char* password) {
  if (retryMS.ready()) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WLAN getrennt - versuche Neuverbindung...");
      WiFi.begin(ssid, password);
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("WLAN wieder verbunden!");
        Serial.print("IP-Adresse: ");
        Serial.println(WiFi.localIP());
      }
    }
  }
}
