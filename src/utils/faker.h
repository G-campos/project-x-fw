#ifndef FAKER_H
#define FAKER_H

#include <Arduino.h>

float  getRandomPower();
float  getRandomVoltage();
float  getRandomAmperage();
float  getRandomEnergy();
float  getRandomTemperature();
int    getRandomSignalIntensity();
String getRandomDirectionRotation();

#endif // FAKER_H