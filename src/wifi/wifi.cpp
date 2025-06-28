#include "wifi.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include "storage/storage.h"
#include "mqtt/mqtt.h"
#include "config.h"

// WIFI config
#define AP_SSID "ESP-CONFIG"

enum WifiState {
    WIFI_NOT_STARTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_AP_MODE
};

WifiState wifiState = WIFI_NOT_STARTED;
String ssid, password;
unsigned long connectionStartTime = 0;
bool credentialsLoaded = false;

DynamicJsonDocument list_wifi_ssid() {
    const char* TAG = "list_wifi_ssid";

    DynamicJsonDocument doc(1024);
    JsonArray ssids = doc.to<JsonArray>();

    int n = WiFi.scanNetworks();
    logger.info(TAG,"Scan done.");

    if (n > 0) {
        logger.infof(TAG,"%d networks found.", n);
        for (int i = 0; i < n; ++i) {
            ssids.add(WiFi.SSID(i));
        }
    } else {
        logger.info(TAG,"Nenhuma rede encontrada.");
    }

    return doc;
}

void wifi_connect_start(const char* ssid, const char* password) {
    const char* TAG = "wifi_connect_start";
    logger.infof(TAG, "Conectando a %s...", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    connectionStartTime = millis();
}

void wifi_init() {
    const char* TAG = "wifi_init";

    if (storage_load_config(config)) {
        credentialsLoaded = true;
        wifi_connect_start(config.ssid.c_str(), config.password.c_str());
        wifiState = WIFI_CONNECTING;
    } else {
        logger.warn(TAG, "Nenhuma credencial salva. Iniciando AP.");
        WiFi.softAP(AP_SSID, NULL);
        wifiState = WIFI_AP_MODE;
    }
}

void wifi_connecting() {
    const unsigned long WIFI_TIMEOUT = 10000;
    const char* TAG = "wifi_connecting";

    if (WiFi.status() == WL_CONNECTED) {
        logger.infof(TAG, "Conectado! IP: %s", WiFi.localIP().toString().c_str());
        wifiState = WIFI_CONNECTED;
        WiFi.softAPdisconnect(true);
        mqtt_init();
    } else if (millis() - connectionStartTime > WIFI_TIMEOUT) {
        logger.error(TAG, "Falha ao conectar. Iniciando AP.");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, NULL);
        wifiState = WIFI_AP_MODE;
    }
}

void wifi_connected() {
    const char* TAG = "wifi_connected";

    if (WiFi.status() != WL_CONNECTED) {
        logger.warn(TAG, "Conexão Wi-Fi perdida. Tentando reconectar...");
        wifiState = WIFI_CONNECTING;
        wifi_connect_start(config.ssid.c_str(), config.password.c_str());
    }
}

void wifi_loop() {
    const char* TAG = "wifi_loop";

    switch (wifiState) {
        case WIFI_NOT_STARTED:
            wifi_init();
            break;

        case WIFI_CONNECTING:
            wifi_connecting();
            break;

        case WIFI_CONNECTED:
            wifi_connected();
            break;

        case WIFI_AP_MODE:
            // logger.info(TAG, "Wi-Fi em modo AP.");
            break;
    }
}