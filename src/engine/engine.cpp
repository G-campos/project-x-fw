#include "engine.h"

#include "config.h"

#define EEPROM_ADDR 0  // Start address for settings

void Engine::begin() {
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_PWM_OUTPUT, OUTPUT);
  
  pinMode(PIN_VOLTAGE_INPUT, INPUT);
  pinMode(PIN_CURRENT_INPUT, INPUT);

  pinMode(PIN_CW_INPUT, INPUT_PULLUP);
  pinMode(PIN_CCW_INPUT, INPUT_PULLUP);
  pinMode(PIN_MULTISPEED_1_INPUT, INPUT_PULLUP);
  pinMode(PIN_MULTISPEED_2_INPUT, INPUT_PULLUP);
  

  // EEPROM.begin(16);
  // loadSettings();
}

void Engine::loop() {
  updateMode();

  if (currentMode != lastMode) {
    uint8_t targetPWM = 0;

    switch (currentMode) {
      case MODE_CW_MIN:
        digitalWrite(PIN_RELAY_1, HIGH);
        digitalWrite(PIN_RELAY_2, LOW);
        targetPWM = minSpeed * 255 / 100;
        break;
      case MODE_CW_M1:
        digitalWrite(PIN_RELAY_1, HIGH);
        digitalWrite(PIN_RELAY_2, LOW);
        targetPWM = multiSpeed1 * 255 / 100;
        break;
      case MODE_CW_M2:
        digitalWrite(PIN_RELAY_1, HIGH);
        digitalWrite(PIN_RELAY_2, LOW);
        targetPWM = multiSpeed2 * 255 / 100;
        break;
      case MODE_CCW_MIN:
        digitalWrite(PIN_RELAY_1, LOW);
        digitalWrite(PIN_RELAY_2, HIGH);
        targetPWM = minSpeed * 255 / 100;
        break;
      case MODE_CCW_M1:
        digitalWrite(PIN_RELAY_1, LOW);
        digitalWrite(PIN_RELAY_2, HIGH);
        targetPWM = multiSpeed1 * 255 / 100;
        break;
      case MODE_CCW_M2:
        digitalWrite(PIN_RELAY_1, LOW);
        digitalWrite(PIN_RELAY_2, HIGH);
        targetPWM = multiSpeed2 * 255 / 100;
        break;
      case MODE_STOPPED:
        rampDown(currentPWM, 0, decelTime);
        digitalWrite(PIN_RELAY_1, LOW);
        digitalWrite(PIN_RELAY_2, LOW);
        currentPWM = 0;
        break;
    }

    if (currentMode != MODE_STOPPED) {
      applyPWM(targetPWM);
    }

    lastMode = currentMode;
  }
}

void Engine::updateMode() {
  bool cw = !digitalRead(PIN_CW_INPUT);
  bool ccw = !digitalRead(PIN_CCW_INPUT);
  bool ms1 = !digitalRead(PIN_MULTISPEED_1_INPUT);
  bool ms2 = !digitalRead(PIN_MULTISPEED_2_INPUT);

  if (cw && !ccw)
    currentMode = ms1 ? MODE_CW_M1 : (ms2 ? MODE_CW_M2 : MODE_CW_MIN);
  else if (ccw && !cw)
    currentMode = ms1 ? MODE_CCW_M1 : (ms2 ? MODE_CCW_M2 : MODE_CCW_MIN);
  else
    currentMode = MODE_STOPPED;
}

void Engine::applyPWM(uint8_t targetPWM) {
  if (targetPWM > currentPWM)
    rampUp(currentPWM, targetPWM, accelTime);
  else if (targetPWM < currentPWM)
    rampDown(currentPWM, targetPWM, decelTime);

  currentPWM = targetPWM;
}

void Engine::rampUp(uint8_t from, uint8_t to, uint16_t duration) {
  if (to <= from) return;
  uint16_t stepDelay = duration / (to - from);
  for (uint8_t pwm = from; pwm <= to; pwm++) {
    analogWrite(PIN_PWM_OUTPUT, pwm);
    delay(stepDelay);
  }
}

void Engine::rampDown(uint8_t from, uint8_t to, uint16_t duration) {
  if (from <= to) return;
  uint16_t stepDelay = duration / (from - to);
  for (int pwm = from; pwm >= to; pwm--) {
    analogWrite(PIN_PWM_OUTPUT, pwm);
    delay(stepDelay);
    if (pwm == 0) break;
  }
}

// -------------------- Getters/Setters ------------------

void Engine::setMinSpeed(uint8_t speed) { 
  minSpeed = constrain(speed, 0, 100); 
  // saveSettings(); 
}
void Engine::setMultiSpeed1(uint8_t speed) { 
  multiSpeed1 = constrain(speed, 0, 100); 
  // saveSettings(); 
}
void Engine::setMultiSpeed2(uint8_t speed) { 
  multiSpeed2 = constrain(speed, 0, 100); 
  // saveSettings(); 
}

void Engine::setAccelerationTime(uint16_t ms) { 
  accelTime = ms; 
  // saveSettings(); 
}
void Engine::setDecelerationTime(uint16_t ms) { 
  decelTime = ms; 
  // saveSettings(); 
}

uint8_t Engine::getMinSpeed() { return minSpeed; }
uint8_t Engine::getMultiSpeed1() { return multiSpeed1; }
uint8_t Engine::getMultiSpeed2() { return multiSpeed2; }

uint16_t Engine::getAccelerationTime() { return accelTime; }
uint16_t Engine::getDecelerationTime() { return decelTime; }

EngineMode Engine::getCurrentMode() { return currentMode; }

float Engine::readVoltage(float maxVoltage) {
  return analogRead(PIN_VOLTAGE_INPUT) * maxVoltage / 1024.0;
}

float Engine::readCurrent() {
  float raw = analogRead(PIN_CURRENT_INPUT);
  return map(raw, 0, 1023, -5000, 5000) / 1000.0;
}

float Engine::getPower() {
  return readVoltage() * readCurrent();
}

// -------------------- EEPROM ------------------

// void Engine::loadSettings() {
//   EEPROM.get(EEPROM_ADDR, minSpeed);
//   EEPROM.get(EEPROM_ADDR + 1, multiSpeed1);
//   EEPROM.get(EEPROM_ADDR + 2, multiSpeed2);
//   EEPROM.get(EEPROM_ADDR + 3, accelTime);
//   EEPROM.get(EEPROM_ADDR + 5, decelTime);
// }

// void Engine::saveSettings() {
//   EEPROM.put(EEPROM_ADDR, minSpeed);
//   EEPROM.put(EEPROM_ADDR + 1, multiSpeed1);
//   EEPROM.put(EEPROM_ADDR + 2, multiSpeed2);
//   EEPROM.put(EEPROM_ADDR + 3, accelTime);
//   EEPROM.put(EEPROM_ADDR + 5, decelTime);
//   EEPROM.commit();
// }
