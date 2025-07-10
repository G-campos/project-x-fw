#ifndef FAKER_H
#define FAKER_H

#include <Arduino.h>

int  getRandomPower();
int  getRandomVoltage();
int  getRandomAmperage();
int  getRandomEnergy();
int  getRandomTemperature();
int  getRandomSpeed();
int    getRandomSignalIntensity();
String getRandomWifiStatus();
String getRandomMqttStatus();
String getRandomDirectionRotation();

#endif // FAKER_H