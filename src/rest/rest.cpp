#include "rest.h"

#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include <SPIFFS.h>

#include "wifi/wifi.h"
#include "storage/storage.h"
#include "config.h"

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(9090);

bool isStaticIp, blankDevice, keepConfigWegpage;
String staticIp, netmask, gateway;
// uint8_t restartConfigWebpage = 0;
uint8_t wifiStatus;

unsigned long connectTimeout;

String getContentType(String filename) {
    if (server.hasArg("download"))
        return "application/octet-stream";
    else if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}

void handleRoot() {
    const char *TAG = "handleRoot";

    logger.info(TAG, "Requisição recebida para '/'");
    File file = FILESYSTEM.open("/index.html.gz", "r");
    if (!file) {
        logger.error(TAG, "Arquivo /index.html.gz não encontrado no SPIFFS!");
        server.send(404, "text/plain", "File not found");
        return;
    }
    logger.debugf(TAG, "Arquivo /index.html.gz aberto. Tamanho: %d bytes", file.size());

    logger.info(TAG, "Enviando /index.html.gz");

    server.streamFile(file, "text/html");
    file.close();
}

void handleWifiConfig() {
    const char *TAG = "handleWifiConfig";

    logger.info(TAG, "Requisição recebida para '/config'");

    String body = server.arg("plain");
    logger.debugf(TAG, "Corpo da requisição: %s", body.c_str());

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        logger.errorf(TAG, "Erro ao desserializar JSON: %s", error.c_str());
        server.send(400, "application/json", "{\"error\":\"JSON inválido\"}");
        return;
    }

    String ssid = doc["ssid"];
    String password = doc["password"];

    logger.infof(TAG, "SSID: %s, Password: %s", ssid.c_str(), password.c_str());

    if (ssid.length() > 0 && password.length() > 0) {
        logger.info(TAG, "Credenciais válidas, tentando conectar...");
        storage_save_wifi(ssid, password);
        server.send(200, "application/json", "{\"status\":\"Conectando...\"}");
    } else {
        logger.error(TAG, "SSID ou senha ausentes");
        server.send(400, "application/json", "{\"error\":\"SSID e senha obrigatórios\"}");
    }
}

void handleWifiScanned() {
    const char *TAG = "handleWifiScanned";

    logger.info(TAG, "Requisição recebida para '/scan'");

    DynamicJsonDocument doc = list_wifi_ssid();
    if (doc.size() > 0) {
        String json;
        serializeJson(doc, json);
        logger.infof(TAG, "Redes encontradas: %s", json.c_str());
        server.send(200, "application/json", json);
    } else {
        logger.info(TAG, "Nenhuma rede encontrada");
        server.send(400, "application/json", "{\"error\":\"Nenhuma rede encontrada\"}");
    }
}

void handleNotFound() {
    const char *TAG = "handleNotFound";
    logger.infof(TAG, "Caminho não encontrado: %s", server.uri());
    server.send(404, "text/plain", "Not found");
}

void rest_router() {
    const char *TAG = "rest_router";

    logger.info(TAG, "Configurando rotas...");

    server.on("/", HTTP_GET, handleRoot);
    // server.serveStatic("/", FILESYSTEM, "/").setDefaultFile("/index.html");
    server.on("/config", HTTP_POST, handleWifiConfig);
    server.on("/scan", HTTP_GET, handleWifiScanned);
    server.onNotFound(handleNotFound);
}

void configureDevice() {
    const char* TAG = "configureDevice";
    logger.info(TAG, "Configuring device...");
    File file = FILESYSTEM.open("/wifi.json", "r");
    int len = file.size();
    char buff[len];
    if (!file) {
        logger.info(TAG, "Failed to open config file, blankDevice true");
        ssid = "";
        password = "";
        isStaticIp = false;
        staticIp = "";
        netmask = "";
        gateway = "";
        blankDevice = true;
        return;
    }
    while (file.available()) {
        file.readBytes(buff, len);
    }
    file.close();
    StaticJsonDocument<512> jsonBuffer;
    deserializeJson(jsonBuffer, buff);
    JsonObject jsonObject = jsonBuffer.as<JsonObject>();
    JsonVariant _ssid = jsonObject["deviceConfiguration"]["ssid"];
    JsonVariant _password = jsonObject["deviceConfiguration"]["password"];
    JsonVariant _isStaticIp = jsonObject["deviceConfiguration"]["isStaticIp"];
    JsonVariant _staticIp = jsonObject["deviceConfiguration"]["staticIp"];
    JsonVariant _netmask = jsonObject["deviceConfiguration"]["netmask"];
    JsonVariant _gateway = jsonObject["deviceConfiguration"]["gateway"];
    ssid = _ssid.as<String>();
    password = _password.as<String>();
    isStaticIp = _isStaticIp.as<bool>();
    staticIp = _staticIp.as<String>();
    netmask = _netmask.as<String>();
    gateway = _gateway.as<String>();

    // if(ssid == "" || (isStaticIp == true && (staticIp == "" || netmask == "" || gateway == "")))

    blankDevice = false;

    logger.debug(TAG, "------ WIFI ------");
    logger.debugf(TAG, "SSID: %S", ssid);
    logger.debugf(TAG, "PASSWORD: %S", password);
    logger.debugf(TAG, "IS_STATIC_IP: %S", isStaticIp);
    logger.debugf(TAG, "STATIC_IP: %S", staticIp);
    logger.debugf(TAG, "NETMASK: %S", netmask);
    logger.debugf(TAG, "GATEWAY: %S", gateway);
    logger.debug(TAG, "------------------");

    logger.info(TAG, "Configure OK");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    const char *TAG = "webSocketEvent";
    switch (type) {
        case WStype_DISCONNECTED:
            logger.debugf(TAG, "[%u] Disconnected!", num);
            break;
        case WStype_CONNECTED:
        {
            IPAddress ip = webSocket.remoteIP(num);
            logger.debugf(TAG, "[%u] Connected from %d.%d.%d.%d url: %s", num, ip[0], ip[1], ip[2], ip[3], payload);
            size_t len;

            DynamicJsonDocument deviceInfo(JSON_ARRAY_SIZE(500));
            deviceInfo["deviceInfo"]["macAddress"] = WiFi.macAddress();
            deviceInfo["deviceInfo"]["ssid"] = ssid;
            deviceInfo["deviceInfo"]["password"] = password;
            deviceInfo["deviceInfo"]["isStaticIp"] = isStaticIp;
            deviceInfo["deviceInfo"]["staticIp"] = staticIp;
            deviceInfo["deviceInfo"]["netmask"] = netmask;
            deviceInfo["deviceInfo"]["gateway"] = gateway;

            len = measureJson(deviceInfo);
            char jsonToSend[len];
            serializeJson(deviceInfo, Serial);
            serializeJson(deviceInfo, jsonToSend, len + 1);
            webSocket.sendTXT(num, jsonToSend, strlen(jsonToSend));
            // webSocket.sendTXT(num, "{Connected}");
        }
            break;
        case WStype_TEXT:
        {
            logger.debugf(TAG, "[%u] get Text: %s", num, payload);

            StaticJsonDocument<512> jsonBuffer;
            deserializeJson(jsonBuffer, payload);
            JsonObject jsonObject = jsonBuffer.as<JsonObject>();

            if (strcmp((const char *)payload, "scanNetworks") == 0) {
                uint8_t n;
                size_t len;
                DynamicJsonDocument networksList(JSON_ARRAY_SIZE(50));
                logger.info(TAG, "scan start");
                n = WiFi.scanNetworks();
                if (n == 0) {
                    logger.info(TAG, "no networks found");
                } else {
                    logger.infof(TAG, "%u networks found", n);
                    for (uint8_t i = 0; i < n; ++i) {
                        // Pruint8_t SSID and RSSI for each network found
                        logger.debugf(TAG, "%u: %S (%i)", i + 1, WiFi.SSID(i), WiFi.RSSI(i));
                        // Serial.print(i + 1);
                        // Serial.print(": ");
                        // Serial.print(WiFi.SSID(i));
                        // Serial.print(" (");
                        // Serial.print(WiFi.RSSI(i));
                        // Serial.println(")");
                        // networks[i] = WiFi.SSID(i);
                        networksList["ssidArray"].add(WiFi.SSID(i));
                        delay(10);
                    }
                }
                len = measureJson(networksList);
                char networksArray[len];
                serializeJson(networksList, Serial);
                // Serial.println("");
                // Serial.println(len);
                serializeJson(networksList, networksArray, len + 1);
                webSocket.sendTXT(num, networksArray, strlen(networksArray));
            }

            if (strcmp((const char *)payload, "ping") == 0) {
                webSocket.sendTXT(num, "{\"pong\":true}");
            }

            if (strcmp((const char *)payload, "deviceConfiguration") == 0) {
                // webSocket.sendTXT(num, "{\"pong\":true}");
                JsonVariant _ssid = jsonObject["deviceConfiguration"]["ssid"];
                JsonVariant _password = jsonObject["deviceConfiguration"]["password"];
                JsonVariant _isStaticIp = jsonObject["deviceConfiguration"]["isStaticIp"];
                JsonVariant _staticIp = jsonObject["deviceConfiguration"]["staticIp"];
                JsonVariant _netmask = jsonObject["deviceConfiguration"]["netmask"];
                JsonVariant _gateway = jsonObject["deviceConfiguration"]["gateway"];
                ssid = _ssid.as<String>();
                password = _password.as<String>();
                isStaticIp = _isStaticIp.as<bool>();
                staticIp = _staticIp.as<String>();
                netmask = _netmask.as<String>();
                gateway = _gateway.as<String>();

                int len = measureJson(jsonObject);
                char buff[len];
                serializeJson(jsonObject, buff, len + 1);
                File file = FILESYSTEM.open("/wifi.json", "w");
                if (!file) {
                    logger.error(TAG, "Wifi file write error");
                    return;
                }
                if (file.print(buff)) {
                    Serial.println("Wifi file was written");
                    Serial.println(buff);
                } else {
                    logger.error(TAG, "File write failed");
                }
                file.close();
                wifi_connect_start(ssid.c_str(), password.c_str());
                blankDevice = false;
                webSocket.sendTXT(num, "{\"saveOk\":true}");
            }

            if (strcmp((const char *)payload, "eraseConfig") == 0) {
                ssid = "";
                password = "";
                isStaticIp = false;
                staticIp = "";
                netmask = "";
                gateway = "";
                FILESYSTEM.remove("/wifi.json");
                blankDevice = true;
                webSocket.sendTXT(num, "{\"eraseOk\":true}");
            }

            if (strcmp((const char *)payload, "startDevice") == 0) {
                configureDevice();
                webSocket.sendTXT(num, "{\"startDeviceOk\":true}");
                blankDevice = false;
                keepConfigWegpage = false;
            }

            if (strcmp((const char *)payload, "testWifi") == 0) {
                if (blankDevice == true) {
                    Serial.println("Cant test blank device");
                    webSocket.sendTXT(num, "{\"testWifiStatus\":\"blankdevice\"}");
                    return;
                }
                Serial.print("Testing connection to: ");
                // WiFi.mode(WIFI_AP_STA);
                Serial.println(ssid);
                Serial.print("Password ");
                Serial.println(password);
                WiFi.disconnect(true);
                WiFi.begin(ssid.c_str(), password.c_str());
                connectTimeout = millis() + (20 * 1000);
                while (true) {
                    wifiStatus = WiFi.status();
                    Serial.printf("(%d).", wifiStatus);
                    if ((wifiStatus == WL_CONNECTED) || (wifiStatus == WL_NO_SSID_AVAIL) ||
                        (wifiStatus == WL_CONNECT_FAILED) || (millis() >= connectTimeout))
                        break;
                    delay(100);
                }
                switch (wifiStatus) {
                    case WL_NO_SSID_AVAIL:
                        logger.info("wifiStatus", "WL_NO_SSID_AVAIL");
                        webSocket.sendTXT(num, "{\"testWifiStatus\":\"WL_NO_SSID_AVAIL\"}");
                        break;
                    case WL_CONNECT_FAILED:
                        logger.info("wifiStatus", "WL_CONNECT_FAILED");
                        webSocket.sendTXT(num, "{\"testWifiStatus\":\"WL_CONNECT_FAILED\"}");
                        break;
                    case WL_CONNECTED:
                        logger.info("wifiStatus", "WL_CONNECTED");
                        webSocket.sendTXT(num, "{\"testWifiStatus\":\"WL_CONNECTED\"}");
                        break;
                    default:
                        logger.info("wifiStatus", "default");
                        webSocket.sendTXT(num, "{\"testWifiStatus\":\"error\"}");
                        break;
                }
                WiFi.disconnect();
            }
        }
            break;
        case WStype_BIN:
        case WStype_PING:
        case WStype_PONG:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}


void rest_init() {
    const char *TAG = "rest_init";

    logger.info(TAG,"Iniciando servico WebSocket...");
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    logger.info(TAG,"Servico WebSocket iniciado.");

    logger.info(TAG, "Iniciando servico REST...");
    rest_router();
    server.begin();
    logger.info(TAG, "Servico REST iniciado.");
}

void rest_loop() {
    server.handleClient();
    webSocket.loop();
}
