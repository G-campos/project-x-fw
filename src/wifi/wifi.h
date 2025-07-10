#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern String ssid, password;

void wifi_init();
void wifi_loop();

void wifi_connect_start(const char* ssid, const char* password);
DynamicJsonDocument list_wifi_ssid();
String getIp();
bool getConnectionStatus();

#endif // WIFI_MANAGER_H
