
#ifndef MQTT_H
#define MQTT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

void mqtt_init();
void mqtt_reconnect();
void mqtt_loop();
void sendTelemetry();

#endif // MQTT_H