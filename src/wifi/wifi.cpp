#include "wifi.h"

#include <storage/storage.h>
#include "WiFi.h"

#include "../config.h"

DynamicJsonDocument list_wifi_ssid() {
    DynamicJsonDocument doc(1024);
    JsonArray ssids = doc.to<JsonArray>();

    int n = WiFi.scanNetworks();
    Serial.println("Scan done.");

    if (n > 0) {
        Serial.printf("%d networks found.\n", n);
        for (int i = 0; i < n; ++i) {
            ssids.add(WiFi.SSID(i));
        }
    } else {
        Serial.println("Nenhuma rede encontrada.");
    }

    return doc;
}

void wifi_connect(const char* ssid, const char* password) {
    Serial.printf("Conectando a %s...\n", ssid);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nConectado! IP: %s\n", WiFi.localIP().toString().c_str());
        storage_save_wifi(ssid, password);
    } else {
        Serial.println("\nFalha na conexão, iniciando modo AP.");
        wifi_start_ap();
    }
}

void wifi_start_ap() {
    Serial.println("Iniciando modo AP...");
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.printf("AP IP: %s\n", WiFi.softAPIP().toString().c_str());
}

void wifi_init() {
    String ssid, password;
    if (storage_load_wifi(ssid, password)) {
        wifi_connect(ssid.c_str(), password.c_str());
    } else {
        wifi_start_ap();
    }
}
