#pragma once
#include <Arduino.h>
#include <WiFiS3.h>

void connectWiFi(const char* ssid, const char* password);
void maintainWiFi(const char* ssid, const char* password);
