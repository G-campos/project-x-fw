#include "ihm.h"

#include <SPI.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>

#include "ihm/screens/main_screen.h"
#include "ihm/screens/menu_screen.h"

#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "utils/faker.h"
#include "gpio/btn.h"
#include "ntp/ntp.h"
#include "config.h"

// Screen Instance Class
TFT_eSPI tft = TFT_eSPI();
QRcode_eSPI qrcode(&tft);
MainScreen mainScreen(&tft);
MenuScreen menuScreen(&tft);

// Screen Manage Params
uint16_t _debounceDelay = 1000;
unsigned long _lastDebounceTime;
bool wasShow = false;
enum Screen screenShowing = SPLASH;

bool isDemo = false;

int valores[MenuScreen::MAX_MENU_ITEMS] = {20, 30, 40, 5, 5, 0, 0};

Btn btnPgm(PIN_BTN_PGM);
Btn btnUp(PIN_BTN_UP);
Btn btnDown(PIN_BTN_DOWN);

// actual state values
String ip;

bool wasConnection = false;

void callNextScreen(Screen sc) {
  screenShowing = sc;
}

void splash_screen() {
  tft.fillScreen(TFT_BLACK);
  
  tft.setCursor(10, 130);
  tft.setTextSize(2);
  tft.setTextColor(BLUE);
  tft.println("SENAI MECATRONICA");
  
  tft.setCursor(50, 180);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println("Projeto Zyron");
  
  delay(3500);

  tft.fillScreen(BLACK);

  // wasConnection ? callNextScreen(MAIN) : callNextScreenn(QRCODE);
  callNextScreen(MAIN);
}

void qrcode_screen() {
	qrcode.create(ip);
}

void ihm_init(){
  const char* TAG = "ihm_init";
  logger.info(TAG, "Inicializando a IHM...");
  
  tft.init();
  tft.setRotation(3); // 0-3
  qrcode.init();
  
  splash_screen();

  btnPgm.init();
  btnUp.init();
  btnDown.init();

  logger.info(TAG, "IHM inicializado.");
}

// Callbacks para menuScreen
int getMenuValue(MenuScreen::MenuItem item) {
  return valores[item];
}

void setMenuValue(MenuScreen::MenuItem item, int value) {
  valores[item] = value;
  logger.debugf("setMenuValue", "Valor atualizado [%d] -> %d\n", item, value);
}

void resetAllValues() {
  for (int i = 0; i < MenuScreen::MAX_MENU_ITEMS; i++) {
    valores[i] = 0;
  }
  logger.debugf("resetAllValues", "Todos os valores foram resetados!");
}

void returnToMainScreen() {
  logger.info("returnToMainScreen", "Voltando para tela principal...");
  wasShow = false;
  screenShowing = MAIN;
}

void ihm_loop(){
  const char* TAG = "ihm_loop";
  
  wasConnection = getConnectionStatus();
  ip = getIp();

  btnPgm.update();
  btnUp.update();
  btnDown.update();

  switch (screenShowing) {
  case MAIN:
    if (!wasShow) {
      logger.info(TAG, "Exibindo MAIN SCREEN");
      tft.fillScreen(TFT_BLACK);
      mainScreen.show();
      wasShow = !wasShow;
    }
    
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
      _lastDebounceTime = millis();

      if (isDemo) {
        mainScreen.updateScreen(
          getTimestamp(),
          String(getRandomVoltage()),
          String(getRandomAmperage()),
          String(getRandomPower()),
          String(getRandomTemperature()),
          getRandomSpeed(),
          getIp(),
          "DESCONECTADO",
          "DESCONECTADO"
        );
      } else {
        mainScreen.updateScreen(
          getTimestamp(),
          String(motor.readVoltage(3.3)),
          String(motor.readCurrent()),
          String(motor.getPower()),
          String(getRandomTemperature()),
          getRandomSpeed(),
          getIp(),
          getConnectionStatus() ? "CONECTADO" : "DESCONECTADO",
          mqttConnectionStatus()
        );
      }
      
    }

    if (!wasConnection) {
      callNextScreen(QRCODE);
      wasShow = false;
      mainScreen.clear();
    }
    
    if (btnPgm.wasPressed()) {
      logger.info(TAG, "Botao PGM pressionado");
      callNextScreen(MENU);
      wasShow = false;
      mainScreen.clear();
    }

	break;
  
  case QRCODE:
    if (!wasShow) {
      logger.info(TAG, "Exibindo QRCODE SCREEN");
      logger.debugf(TAG, "IP: %S", ip);
	    tft.fillScreen(TFT_BLACK);
      qrcode_screen();
      wasShow = true;

    }

  	if (wasConnection) {
      callNextScreen(MAIN);
      wasShow = false;
    }

    if (btnPgm.wasPressed()) {
      logger.info(TAG, "Botao PGM pressionado");
      callNextScreen(MENU);
      wasShow = false;
    }
	break;

  case MENU:
    if (!wasShow) {
      logger.info(TAG, "Exibindo MENU SCREEN");
      tft.fillScreen(TFT_BLACK);
      menuScreen.show();
      wasShow = true;
	    menuScreen.setCallbacks(
        getMenuValue,
        setMenuValue,
        resetAllValues,
        returnToMainScreen
      );
    } else {
      if (btnPgm.wasPressed()) {
        logger.info(TAG, "Botao PGM pressionado");
        menuScreen.select();
      }

      if (btnUp.wasPressed()) {
        logger.info(TAG, "Botao UP pressionado");
        menuScreen.navigateUp();
      }

      if (btnDown.wasPressed()) {
        logger.info(TAG, "Botao DOWN pressionado");
        menuScreen.navigateDown();
      }
    }

	if (!wasConnection) {
      callNextScreen(QRCODE);
      wasShow = false;
  	}
	break;

  default:
    if (!wasShow) {
		logger.info(TAG, "Exibindo SPLASH SCREEN");
		wasShow = true;
		tft.fillScreen(TFT_BLACK);
		splash_screen();
	} 
	break;
  }
}
