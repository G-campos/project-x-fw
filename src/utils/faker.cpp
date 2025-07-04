#include "faker.h"

void faker_init() { randomSeed(analogRead(0)); }

float getRandomPower()              { return random(0, 100001) / 100.0; }
float getRandomVoltage()            { return random(10001, 30001) / 100.0; }
float getRandomAmperage()           { return random(0, 3001) / 100.0; }
float getRandomEnergy()             { return random(0, 100) / 100.0; }
float getRandomTemperature()        { return random(0, 10001) / 100.0; }
int   getRandomSignalIntensity()    { return random(0, 11); }

String getRandomDirectionRotation() { return random(0, 2) == 0 ? "left" : "right"; }