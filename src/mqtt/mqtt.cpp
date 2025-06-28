#include "mqtt.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

const char* mqtt_server = ""; 
const int mqtt_port = 1883;
const char* token = "";  // From ThingsBoard device

// Variáveis globais
WiFiClient wifiClient;
PubSubClient client(wifiClient);
unsigned long lastSend = 0;

void mqtt_init() {
  client.setServer(mqtt_server, mqtt_port);
  randomSeed(analogRead(0));
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

float getRandomPower()       { return random(0, 100001) / 100.0; }
float getRandomVoltage()     { return random(10001, 30001) / 100.0; }
float getRandomAmperage()    { return random(0, 3001) / 100.0; }
float getRandomEnergy()      { return random(0, 100) / 100.0; }
float getRandomTemperature() { return random(0, 10001) / 100.0; }
int   getRandomSignalIntensity() { return random(0, 11); }

String getRandomDirectionRotation() {
  return random(0, 2) == 0 ? "left" : "right";
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
