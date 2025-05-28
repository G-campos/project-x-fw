#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

bool storage_load_wifi(String &ssid, String &password);
bool storage_save_wifi(const String &ssid, const String &password);

#endif // STORAGE_H
