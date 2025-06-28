#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

#include "utils/logger.h"
#include "storage/storage.h"

extern Logger logger;
extern Config config;

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

#endif // CONFIG_H