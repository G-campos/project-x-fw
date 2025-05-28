#include "storage.h"
#include <LittleFS.h>

#define WIFI_CONFIG_FILE "/wifi.cfg"

bool storage_load_wifi(String &ssid, String &password) {
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS");
        return false;
    }

    File file = LittleFS.open(WIFI_CONFIG_FILE, "r");
    if (!file || file.isDirectory()) {
        Serial.println("Failed to open wifi.cfg");
        return false;
    }

    ssid = file.readStringUntil('\n');
    ssid.trim();
    password = file.readStringUntil('\n');
    password.trim();

    file.close();
    return ssid.length() > 0;
}

bool storage_save_wifi(const String &ssid, const String &password) {
    if (ssid.isEmpty() || password.isEmpty()) return false;

    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS");
        return false;
    }

    File file = LittleFS.open(WIFI_CONFIG_FILE, "w");
    if (!file) {
        Serial.println("Failed to open wifi.cfg for writing");
        return false;
    }

    file.println(ssid);
    file.println(password);
    file.close();
    return true;
}