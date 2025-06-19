#include <Arduino.h>
#include "esp_partition.h"

#include "wifi/wifi.h"
#include "rest/rest.h"
#include "storage/storage.h"
#include "gpio/display.h"

#include "config.h"

const char* TAG = "main";

Logger logger;

bool restStarted = false;

void setup() {
    logger.begin(115200);
    logger.setLogLevel(LOG_DEBUG);
    logger.setTag(TAG);

    logger.info("setup", "Project X Run...");
    logger.info("setup", "Iniciando sistema...");
    logger.debug("setup", "Debug ativado.");
    logger.warn("setup", "Este é um aviso.");
    logger.error("setup", "Erro de inicialização!");

    // Inicializa Armazenamento
    list_partitions();
    storage_init();

    // Inicia Wi-Fi apos servico rest completar inicialização
    wifi_init();
    
    // Inicializa servidor REST para configuração
    rest_init();

    // Inicializa o display
    display_init();
  }
  
void loop() {
  wifi_loop();

  rest_loop();

  display_loop();
}
