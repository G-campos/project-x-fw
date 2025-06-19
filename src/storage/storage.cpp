#include "storage.h"
#include "config.h"

#define WIFI_CONFIG_FILE "/wifi.cfg"

void list_partitions() {
  const esp_partition_t   *partition;
  esp_partition_iterator_t partition_iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  logger.debug("listPartitions","Name                 | Type  | SubType | Offset     | Size");
  logger.debug("listPartitions","---------------------+-------+---------+------------+-----------");
  while (partition_iterator != NULL) {
    partition = esp_partition_get(partition_iterator);
    logger.debugf("listPartitions","%-20s | 0x%02x  | 0x%02x    | 0x%08x | 0x%08x",
                  partition->label,
                  partition->type,
                  partition->subtype,
                  partition->address,
                  partition->size);
    partition_iterator = esp_partition_next(partition_iterator);
  }

  esp_partition_iterator_release(partition_iterator);
}

void storage_init() {
    const char* TAG = "storage_init";
    if (!FILESYSTEM.begin(true, "/spiffs", 10U, "spiffs")) {
        logger.error(TAG, "Falha ao montar SPIFFS!");
    } else {
        logger.info(TAG, "SPIFFS montado com sucesso.");
    }
}

bool storage_load_wifi(String &ssid, String &password) {
    const char* TAG = "storage_load_wifi";

    File file = FILESYSTEM.open(WIFI_CONFIG_FILE, "r");
    if (!file || file.isDirectory()) {
        logger.error(TAG, "Failed to open wifi.cfg");
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
    const char* TAG = "storage_save_wifi";
    
    if (ssid.isEmpty() || password.isEmpty()) return false;

    File file = FILESYSTEM.open(WIFI_CONFIG_FILE, "w");
    if (!file) {
        logger.error(TAG, "Failed to open wifi.cfg for writing");
        return false;
    }

    file.println(ssid);
    file.println(password);
    file.close();
    return true;
}

void storage_clear_wifi() {
    const char* TAG = "storage_clear_wifi";
    
    File file = FILESYSTEM.open(WIFI_CONFIG_FILE, "w");
    if (!file) {
        logger.error(TAG, "Failed to open wifi.cfg for writing");
    }

    file.print("");
    file.close();
}