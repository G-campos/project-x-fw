#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <TFT_eSPI.h>
#include <SPI.h>

class MainScreen {
  private:
    TFT_eSPI *display;

  	void drawStaticLayout();
	  void drawBox(int x, int y);

   	void updateDateTime(String datetime);
    void updateVoltage(String voltage);
    void updateCurrent(String current);
    void updatePower(String power);
    void updateTemperature(String temperature);
    void updateSpeed(int percent);
    void updateIP(String ip);
    void updateWifiStatus(String status);
    void updateMQTTStatus(String status);

  public:
    MainScreen(TFT_eSPI *display);
    
    void show();
	  void clear();
	
    void updateScreen(String datetime, String voltage, String amperage, String power, String temperature, int percent, String ip, String wifiStatus, String mqttStatus);
};

#endif
