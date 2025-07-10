#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

#include "utils/logger.h"
#include "storage/storage.h"
#include "engine/engine.h"

extern Logger logger;
extern Config config;
extern Engine motor;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define WHITE          0xFFFF
#define YELLOW         0xFFE0
#define RED            0xF800
#define BLUE           0x001F
#define BLACK          0x0000

#define ON             0x07E0
#define OFF            0xF800

// Pins
#define PIN_BTN_PGM    39
#define PIN_BTN_UP     35
#define PIN_BTN_DOWN   34

// Pins Engine Out
#define PIN_RELAY_1              28
#define PIN_RELAY_2              30
#define PIN_PWM_OUTPUT           3  // revisar

// Pins Engine Input
#define PIN_VOLTAGE_INPUT        33 // revisar
#define PIN_CURRENT_INPUT        32 // revisar

// Pins Btns extras
#define PIN_CHANGE_BUTTON        27 
#define PIN_ENTER_BUTTON         14
#define PIN_CCW_INPUT            16 // revisar
#define PIN_CW_INPUT             16 // revisar
#define PIN_MULTISPEED_1_INPUT   13
#define PIN_MULTISPEED_2_INPUT   12// revisar


// Pins Display TFT
// #define TFT_BL   21            // LED back-light control pin
// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   15  // Chip select control pin
// #define TFT_DC    2  // Data Command control pin
// #define TFT_RST   5  // Reset pin (could connect to RST pin)
// #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
// #define TOUCH_CS 33     // Chip select pin (T_CS) of touch screen

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240

#define AP_SSID "ESP-CONFIG"

#endif // CONFIG_H