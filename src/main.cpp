#include <Arduino.h>
#include "wifi/wifi.h"
#include "rest/rest.h"
#include "gpio/display.h"

void setup() {
    Serial.begin(115200);
    Serial.println("\nProject X Run...");

    // Inicializa WiFi Manager
    wifi_init();
    
    // Inicializa servidor REST para configuração
    rest_init();

    display_init();
}

void loop() {
    display_loop();
}
