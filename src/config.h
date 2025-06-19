#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

#include "utils/logger.h"

extern Logger logger;

// WIFI config

#define AP_SSID "ESP-CONFIG"
#define AP_PASS ""
#define WIFI_STORAGE_NAMESPACE "wifi_config"

#ifndef LED_BUILTIN
#define LED_BUILTIN 99
#endif

#define WHITE          0xFFFF
#define YELLOW         0xFFE0
#define RED            0xF800
#define BLUE           0x001F
#define BLACK          0x0000

#define ON         0x07E0
#define OFF      0xF800

// THINGSBOARD CONFIG

#if defined(ESP8266)
#define THINGSBOARD_ENABLE_PROGMEM 0
#endif

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] = "YOUR_ACCESS_TOKEN";

// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] = "demo.thingsboard.io";
// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port.
constexpr uint16_t THINGSBOARD_PORT = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;

// Baud rate for the debugging serial connection.
// If the Serial output is mangled, ensure to change the monitor speed accordingly to this variable
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

// Maximum amount of attributs we can request or subscribe, has to be set both in the ThingsBoard template list and Attribute_Request_Callback template list
// and should be the same as the amount of variables in the passed array. If it is less not all variables will be requested or subscribed
constexpr size_t MAX_ATTRIBUTES = 3U;

constexpr uint64_t REQUEST_TIMEOUT_MICROSECONDS = 5000U * 1000U;

// Attribute names for attribute request and attribute updates functionality

constexpr const char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr const char LED_MODE_ATTR[] = "ledMode";
constexpr const char LED_STATE_ATTR[] = "ledState";

#endif // CONFIG_H