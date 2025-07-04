#include "mqtt.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

#include "utils/faker.h"

const char* mqtt_server = ""; 
const int mqtt_port = 1883;
const char* token = "";  // From ThingsBoard device

// Variáveis globais
WiFiClient wifiClient;
PubSubClient client(wifiClient);              // agora compartilhada entre as funções
unsigned long lastSend = 0;

void mqtt_init() {
  client.setServer(mqtt_server, mqtt_port);
}

void mqtt_reconnect() {
  const char* TAG = "mqtt_reconnect";
  while (!client.connected()) {
    logger.info(TAG, "Attempting MQTT connection...");
    if (client.connect("ESP32Client", token, NULL)) {
      logger.info(TAG, "connected");
    } else {
      logger.errorf(TAG, "failed, rc=%d", client.state());
      logger.warn(TAG, "try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_loop() {
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();

  if (millis() - lastSend > 5000) {
    sendTelemetry();
    lastSend = millis();
  }
}

void sendTelemetry() {
  String payload = "{"
    "\"power\":" + String(getRandomPower(), 2) + "," +
    "\"voltage\":" + String(getRandomVoltage(), 2) + "," +
    "\"energy\":" + String(getRandomEnergy(), 2) + "," +
    "\"amperage\":" + String(getRandomAmperage(), 2) + "," +
    "\"temperature\":" + String(getRandomTemperature(), 2) + "," +
    "\"signalIntensity\":" + String(getRandomSignalIntensity()) + "," +
    "\"directionRotation\":\"" + getRandomDirectionRotation() + "\"" +
  "}";

  Serial.println("Publishing telemetry: " + payload);
  client.publish("v1/devices/me/telemetry", payload.c_str());
}
