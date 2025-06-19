#include "rest.h"

#include <WebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include "wifi/wifi.h"
#include "storage/storage.h"
#include "config.h"

WebServer server(80);

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void handleRoot() {
    const char* TAG = "handleRoot";

    logger.info(TAG,"Requisição recebida para '/'");
    File file = FILESYSTEM.open("/index.html", "r");
    if (!file) {
        logger.error(TAG,"Arquivo /index.html não encontrado no SPIFFS!");
        server.send(404, "text/plain", "index.html não encontrado");
        return;
    }
    logger.debugf(TAG, "Arquivo index.html aberto. Tamanho: %d bytes", file.size());

    logger.info(TAG,"Enviando /index.html");

    size_t sent = server.streamFile(file, getContentType(file.name()));
    file.close();

    // server.send(200, getContentType(file.name()), "/index.html");
}

void handleWifiConfig() {
    const char* TAG = "handleWifiConfig";

    logger.info(TAG,"Requisição recebida para '/config'");

    String body = server.arg("plain");
    logger.debugf(TAG,"Corpo da requisição: %s\n", body.c_str());

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        logger.errorf(TAG,"Erro ao desserializar JSON: %s\n", error.c_str());
        server.send(400, "application/json", "{\"error\":\"JSON inválido\"}");
        return;
    }

    String ssid = doc["ssid"];
    String password = doc["password"];

    logger.infof(TAG,"SSID: %s, Password: %s\n", ssid.c_str(), password.c_str());

    if (ssid.length() > 0 && password.length() > 0) {
        logger.info(TAG,"Credenciais válidas, tentando conectar...");
        storage_save_wifi(ssid.c_str(), password.c_str());
        server.send(200, "application/json", "{\"status\":\"Conectando...\"}");
        ESP.restart();
    } else {
        logger.error(TAG,"SSID ou senha ausentes");
        server.send(400, "application/json", "{\"error\":\"SSID e senha obrigatórios\"}");
    }
}

void handleWifiScanned() {
    const char* TAG = "handleWifiScanned";

    logger.info(TAG,"Requisição recebida para '/scan'");

    DynamicJsonDocument doc = list_wifi_ssid();
    if (doc.size() > 0) {
        String json;
        serializeJson(doc, json);
        logger.infof(TAG,"Redes encontradas: %s\n", json.c_str());
        server.send(200, "application/json", json);
    } else {
        logger.info(TAG,"Nenhuma rede encontrada");
        server.send(400, "application/json", "{\"error\":\"Nenhuma rede encontrada\"}");
    }
}

void handleNotFound() {
    const char* TAG = "handleNotFound";
    logger.infof(TAG,"Caminho não encontrado: %s\n", server.uri());
    server.send(404, "text/plain", "Not found");
}

void rest_router() {
    const char* TAG = "rest_router";
    
    logger.info(TAG,"Configurando rotas...");

    server.on("/", HTTP_GET, handleRoot);
    // server.serveStatic("/", FILESYSTEM, "/").setDefaultFile("/index.html");
    server.on("/config", HTTP_POST, handleWifiConfig);
    server.on("/scan", HTTP_GET, handleWifiScanned);
    server.onNotFound(handleNotFound);

}

void rest_init() {
    const char* TAG = "rest_init";

    logger.info(TAG,"Iniciando servidor REST...");
    rest_router();
    server.begin();
    logger.info(TAG,"Servidor REST iniciado.");
}

void rest_loop() {
    server.handleClient();
}
