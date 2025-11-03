#pragma once
#include <Arduino.h>
#include <Client.h>  // Basis von WiFiClient/EthernetClient/etc.

namespace UbidotsRaw {

// Baut ein einfaches JSON: {"temp": <T>, "hum": <H>}
String makeBodyTempHum(float tempC, float humPct);

// POST /api/v1.6/devices/<deviceLabel> (HTTP/80)
// host: "industrial.api.ubidots.com" oder "things.ubidots.com"
// token: X-Auth-Token
// body:  gültiges JSON (z.B. aus makeBodyTempHum)
bool postDevice(Client& net,
                const char* host,
                const char* deviceLabel,
                const char* token,
                const String& body,
                uint16_t port = 80,
                uint32_t respTimeoutMs = 5000);

} // namespace UbidotsRaw
