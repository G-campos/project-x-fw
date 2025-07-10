#include "faker.h"

void faker_init() { randomSeed(analogRead(0)); }

int getRandomPower()              { return random(0, 10001); }
int getRandomVoltage()            { return random(0, 301); }
int getRandomAmperage()           { return random(0, 3001); }
int getRandomEnergy()             { return random(0, 100); }
int getRandomTemperature()        { return random(0, 1001); }
int getRandomSpeed()              { return random(0, 101) ;}
int getRandomSignalIntensity()    { return random(0, 11); }

String getRandomWifiStatus() { return random(0, 2) == 0 ? "DESCONECTADO" : "CONECTADO"; }
String getRandomMqttStatus() { return random(0, 2) == 0 ? "CONECTADO" : "DESCONECTADO"; }

String getRandomDirectionRotation() { return random(0, 2) == 0 ? "left" : "right"; }