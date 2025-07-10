#include "main_screen.h"

// Cores personalizadas
#define BG_COLOR      TFT_BLACK
#define PANEL_COLOR   0x0010  // Azul escuro personalizado
#define TEXT_COLOR    TFT_WHITE

// last states cache
String lastDatetime;
String lastVoltage;
String lastCurrent;
String lastPower;
String lastTemperature;
int lastSpeed;
String lastIP;
String lastWiFiStatus;
String lastMQTTStatus;

MainScreen::MainScreen(TFT_eSPI *tft) {
  this->display = tft;
}

void MainScreen::clear() {
  display->fillScreen(BG_COLOR);
}

void MainScreen::show() {
    display->fillScreen(BG_COLOR);
    drawStaticLayout();
}

void MainScreen::drawStaticLayout() {
  // Header
  display->setTextDatum(TL_DATUM);
  display->setTextSize(2);
  display->setTextColor(TEXT_COLOR, BG_COLOR);
  display->setCursor(10, 10);
  display->print("00/00/0000,00:00:00");

  // Data boxes
  drawBox(10, 40);     // Tensão
  drawBox(170, 40);    // Corrente
  drawBox(10, 120);    // Potência
  drawBox(170, 120);   // Temperatura

  // Speed bar
  display->fillRoundRect(10, 200, 240, 40, 10, PANEL_COLOR);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextDatum(TL_DATUM);
  display->setTextSize(2);
  display->setCursor(20, 212);
  display->print("VELOCIDADE:");

  // Connection info (IP/WIFI/MQTT)
  display->setTextDatum(TL_DATUM);
  display->setTextColor(TEXT_COLOR, BG_COLOR);
  display->setTextSize(1);
  display->setCursor(10, 260);
  display->print("IP:    ");
  display->setCursor(10, 270);
  display->print("WIFI:  ");
  display->setCursor(10, 280);
  display->print("MQTT:  ");
}

void MainScreen::drawBox(int x, int y) {
  display->fillRoundRect(x, y, 140, 70, 10, PANEL_COLOR);
}

void MainScreen::updateDateTime(String datetime) {
  display->fillRect(10, 10, 240, 24, BG_COLOR);
  display->setCursor(10, 10);
  display->setTextColor(TEXT_COLOR, BG_COLOR);
  display->setTextSize(2);
  display->print(datetime);
}

void MainScreen::updateVoltage(String value) {
  display->setTextDatum(TL_DATUM);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextSize(3);
  display->setCursor(20, 55);
  display->print(value + "V");
}

void MainScreen::updateCurrent(String value) {
  display->setTextDatum(TL_DATUM);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextSize(3);
  display->setCursor(180, 55);
  display->print(value + "A");
}

void MainScreen::updatePower(String value) {
  display->setTextDatum(TL_DATUM);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextSize(3);
  display->setCursor(20, 135);
  display->print(value + "W");
}

void MainScreen::updateTemperature(String value) {
  display->setTextDatum(TL_DATUM);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextSize(3);
  display->setCursor(180, 135);
  display->print(value + "C");
}

void MainScreen::updateSpeed(int percent) {
  display->fillRect(230, 210, 60, 24, PANEL_COLOR);
  display->setCursor(235, 212);
  display->setTextColor(TEXT_COLOR, PANEL_COLOR);
  display->setTextSize(2);
  display->print(String(percent) + "%");
}

void MainScreen::updateIP(String ip) {
  display->fillRect(90, 260, 220, 20, BG_COLOR);
  display->setCursor(90, 260);
  display->setTextSize(1);
  display->setTextColor(TEXT_COLOR, BG_COLOR);
  display->println(ip);
}

void MainScreen::updateWifiStatus(String status) {
  display->fillRect(90, 270, 220, 20, BG_COLOR);
  display->setCursor(90, 270);
  display->setTextSize(1);

  if (status == "CONECTADO") {
    display->setTextColor(TFT_GREEN, BG_COLOR);
  } else {
    display->setTextColor(TFT_RED, BG_COLOR);
  }
  
  display->println(status);
}

void MainScreen::updateMQTTStatus(String status) {
  display->fillRect(90, 280, 220, 20, BG_COLOR);
  display->setCursor(90, 280);
  display->setTextSize(1);
  
  if (status == "CONECTADO") {
    display->setTextColor(TFT_GREEN, BG_COLOR);
  } else {
    display->setTextColor(TFT_RED, BG_COLOR);
  }

  display->println(status);
}

void MainScreen::updateScreen(String datetime, String voltage, String amperage, String power, String temperature, int percent, String ip, String wifiStatus, String mqttStatus) {
	if (datetime != lastDatetime) {
		lastDatetime = datetime;
		updateDateTime(datetime);
	}

	if (voltage != lastVoltage) {
		lastVoltage = voltage;
		updateVoltage(voltage);
	}

	if (amperage != lastCurrent) {
		lastCurrent = amperage;
		updateCurrent(amperage);
	}

	if (power != lastPower) {
		lastPower = power;
		updatePower(power);
	}

	if (temperature != lastTemperature) {
		lastTemperature = temperature;
		updateTemperature(temperature);
	}

	if (percent != lastSpeed) {
		lastSpeed = percent;
		updateSpeed(percent);
	}

	if (ip != lastIP) {
		lastIP = ip;
		updateIP(ip);
	}
	
	if (wifiStatus != lastWiFiStatus) {
		lastWiFiStatus = wifiStatus;
		updateWifiStatus(wifiStatus);
	}
	
	if (mqttStatus != lastMQTTStatus) {
		lastMQTTStatus = mqttStatus;
		updateMQTTStatus(mqttStatus);
	}
}