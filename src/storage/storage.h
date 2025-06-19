#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <SPIFFS.h>

#define FILESYSTEM SPIFFS

// void html_load();
void list_partitions();
void storage_init();
bool storage_load_wifi(String &ssid, String &password);
bool storage_save_wifi(const String &ssid, const String &password);

#endif // STORAGE_H
