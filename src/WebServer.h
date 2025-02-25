#pragma once

#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <WiFi.h>

void setupWiFiAP();
void handleClient();
void sendFile(WiFiClient &client, const char *filename, const char *contentType);

#endif // WIFI_SERVER_H
