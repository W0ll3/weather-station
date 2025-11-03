#include "UbidotsRaw.h"

namespace UbidotsRaw {

String makeBodyTempHum(float tempC, float humPct) {
  // Ergibt: {"temp": 21.5, "hum": 44.2}
  String b = "{";
  b += "\"temp\": "; b += String(tempC, 1);
  b += ", \"hum\": "; b += String(humPct, 1);
  b += "}";
  return b;
}

bool postDevice(Client& net,
                const char* host,
                const char* deviceLabel,
                const char* token,
                const String& body,
                uint16_t port,
                uint32_t respTimeoutMs)
{
  // 1) TCP aufbauen (wenn nicht extern schon geöffnet)
  if (!net.connect(host, port)) {
    Serial.println(F("[UbidotsRaw] TCP connect fehlgeschlagen"));
    return false;
  }

  // 2) HTTP-Request senden
  String path = String("/api/v1.6/devices/") + deviceLabel;

  net.print(F("POST ")); net.print(path); net.println(F(" HTTP/1.1"));
  net.print(F("Host: ")); net.println(host);
  net.print(F("X-Auth-Token: ")); net.println(token);
  net.println(F("Content-Type: application/json"));
  net.print  (F("Content-Length: ")); net.println(body.length());
  net.println(F("Connection: close"));
  net.println();
  net.print(body);

  // 3) Erste Antwortzeile lesen (Status)
  unsigned long t0 = millis();
  String status;
  while (net.connected() && millis() - t0 < respTimeoutMs) {
    if (net.available()) { status = net.readStringUntil('\n'); break; }
  }
  // Rest verwerfen, sauber schließen
  while (net.available()) net.read();
  net.stop();

  // 4) Code extrahieren
  int code = -1;
  if (status.startsWith("HTTP/1.1 ")) code = status.substring(9, 12).toInt();

  Serial.print(F("[UbidotsRaw] Antwort: ")); Serial.println(status);
  return (code == 200 || code == 201 || code == 202);
}

} // namespace UbidotsRaw
