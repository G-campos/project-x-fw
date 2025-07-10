#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
// #include <EEPROM.h>

enum EngineMode {
  MODE_STOPPED,
  MODE_CW_MIN,
  MODE_CW_M1,
  MODE_CW_M2,
  MODE_CCW_MIN,
  MODE_CCW_M1,
  MODE_CCW_M2
};

class Engine {
public:
  void begin();
  void loop();

  void setMinSpeed(uint8_t speed);
  void setMultiSpeed1(uint8_t speed);
  void setMultiSpeed2(uint8_t speed);
  void setAccelerationTime(uint16_t ms);
  void setDecelerationTime(uint16_t ms);

  uint8_t getMinSpeed();
  uint8_t getMultiSpeed1();
  uint8_t getMultiSpeed2();
  uint16_t getAccelerationTime();
  uint16_t getDecelerationTime();
  EngineMode getCurrentMode();

  float readVoltage(float maxVoltage = 34.0);
  float readCurrent();
  float getPower(); // P = V * I

private:
  void applyPWM(uint8_t targetPWM);
  void rampUp(uint8_t from, uint8_t to, uint16_t duration);
  void rampDown(uint8_t from, uint8_t to, uint16_t duration);
  void updateMode();

  // void loadSettings();
  // void saveSettings();

  uint8_t minSpeed = 0;
  uint8_t multiSpeed1 = 0;
  uint8_t multiSpeed2 = 0;
  uint16_t accelTime = 1000;
  uint16_t decelTime = 10000;

  uint8_t currentPWM = 0;
  EngineMode currentMode = MODE_STOPPED;
  EngineMode lastMode = MODE_STOPPED;
};

#endif