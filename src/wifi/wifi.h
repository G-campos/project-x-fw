#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

void wifi_init();
void wifi_loop();
DynamicJsonDocument list_wifi_ssid();

#endif // WIFI_MANAGER_H
