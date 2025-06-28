#include "storage.h"
#include "config.h"

void list_partitions() {
  const esp_partition_t *partition;
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  logger.debug("listPartitions", "Name                 | Type  | SubType | Offset     | Size");
  logger.debug("listPartitions", "---------------------+-------+---------+------------+-----------");
  while (it != NULL) {
    partition = esp_partition_get(it);
    logger.debugf("listPartitions", "%-20s | 0x%02x  | 0x%02x    | 0x%08x | 0x%08x",
                  partition->label, partition->type, partition->subtype,
                  partition->address, partition->size);
    it = esp_partition_next(it);
  }
  esp_partition_iterator_release(it);
}

void storage_init() {
  const char* TAG = "storage_init";
  if (!FILESYSTEM.begin(true, "/spiffs", 10U, "spiffs")) {
    logger.error(TAG, "Falha ao montar SPIFFS!");
  } else {
    logger.info(TAG, "SPIFFS montado com sucesso.");
  }
}

bool storage_load_config(Config &config) {
  const char* TAG = "storage_load_config";

  File file = FILESYSTEM.open(CONFIG_FILE, "r");
  if (!file || file.isDirectory()) {
    logger.error(TAG, "Não foi possível abrir configs.json");
    return false;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    logger.errorf(TAG, "Erro ao fazer parsing do JSON: %s", error.c_str());
    return false;
  }

  config.device_id         = doc["device_id"] | "";
  config.ssid              = doc["network"]["ssid"] | "";
  config.password          = doc["network"]["password"] | "";
  config.server_http_url   = doc["cloud"]["server_http_url"] | "";
  config.server_mqtt_url   = doc["cloud"]["server_mqtt_url"] | "";
  config.api_key           = doc["cloud"]["api_key"] | "";
  config.interval_ms       = doc["data_reporting"]["interval_ms"] | 5000;
  config.format            = doc["data_reporting"]["format"] | "json";

  return true;
}

bool storage_save_config(const Config &config) {
  const char* TAG = "storage_save_config";

  File file = FILESYSTEM.open(CONFIG_FILE, "w");
  if (!file) {
    logger.error(TAG, "Erro ao abrir configs.json para escrita");
    return false;
  }

  StaticJsonDocument<512> doc;

  doc["device_id"] = config.device_id;

  JsonObject network = doc.createNestedObject("network");
  network["ssid"] = config.ssid;
  network["password"] = config.password;

  JsonObject cloud = doc.createNestedObject("cloud");
  cloud["server_http_url"] = config.server_http_url;
  cloud["server_mqtt_url"] = config.server_mqtt_url;
  cloud["api_key"] = config.api_key;

  JsonObject data_reporting = doc.createNestedObject("data_reporting");
  data_reporting["interval_ms"] = config.interval_ms;
  data_reporting["format"] = config.format;

  if (serializeJson(doc, file) == 0) {
    logger.error(TAG, "Falha ao salvar o JSON no arquivo");
    file.close();
    return false;
  }

  file.close();
  return true;
}

void storage_clear() {
  const char* TAG = "storage_clear";

  if (FILESYSTEM.exists(CONFIG_FILE)) {
    FILESYSTEM.remove(CONFIG_FILE);
    logger.info(TAG, "Arquivo configs.json removido.");
  } else {
    logger.warn(TAG, "configs.json não existe.");
  }
}
