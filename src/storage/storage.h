#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define FILESYSTEM SPIFFS
#define CONFIG_FILE "/configs.json"
#define WIFI_FILE "/wifi.json"

struct Config {
  String ssid;
  String password;
  // String device_id;
  // String server_http_url;
  // String server_mqtt_url;
  // String api_key;
  // uint32_t interval_ms;
  // String format;
};

void list_partitions();
void storage_init();
bool storage_load_config(Config &config);
bool storage_save_config(Config &config);
bool storage_save_wifi(String &ssid, String &password);
void storage_clear();

#endif // STORAGE_H
