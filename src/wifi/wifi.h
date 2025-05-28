#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

void wifi_init();
void wifi_connect(const char* ssid, const char* password);
void wifi_start_ap();
DynamicJsonDocument list_wifi_ssid();

#endif // WIFI_MANAGER_H
