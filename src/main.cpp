#include <Arduino.h>
#include "esp_partition.h"

#include "wifi/wifi.h"
#include "rest/rest.h"
#include "storage/storage.h"
#include "ihm/ihm.h"

#include "utils/faker.h"

#include "config.h"

Logger logger;
Config config;
Engine motor;

void setup() {
    const char* TAG = "setup";
    
    logger.begin(115200);
    logger.setLogLevel(LOG_DEBUG);
    logger.setTag(TAG);

    logger.info(TAG, "Project Zyron Run...");
    logger.info(TAG, "Iniciando sistema...");
    logger.debug(TAG, "Debug ativado.");
    logger.warn(TAG, "Este é um aviso.");
    logger.error(TAG, "Erro de inicialização!");

    //load fake data
    randomSeed(analogRead(0));

    // Inicializa Armazenamento
    list_partitions();
    storage_init();

    if (storage_load_config(config)) {
      logger.info(TAG, "Configuração carregada:");
      logger.infof(TAG, "SSID: %s", config.ssid);
      // logger.infof(TAG, "MQTT: %s", config.server_mqtt_url);
    } else {
      logger.error(TAG, "Falha ao carregar configuração.");
    }

    // Inicializa o controle do motor
    motor.begin();
    // valores base
    motor.setMinSpeed(30); 
    motor.setMultiSpeed1(60);
    motor.setMultiSpeed2(90);

    // Inicializa servicos Wi-Fi
    wifi_init();
    
    // Inicializa servico REST para configuração
    // rest_init();

    // Inicializa a ihm
    ihm_init();
  }
  
void loop() {
  motor.loop();

  wifi_loop();

  // rest_loop();

  ihm_loop();
}
