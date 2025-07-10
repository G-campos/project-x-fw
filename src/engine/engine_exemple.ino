#include "engine.h"

Engine motor;

void setup() {
  Serial.begin(115200);
  motor.begin();

  motor.setMinSpeed(30);
  motor.setMultiSpeed1(60);
  motor.setMultiSpeed2(90);
}

void loop() {
  motor.loop();

  float voltage = motor.readVoltage();
  float current = motor.readCurrent();
  float power = motor.getPower();

  Serial.print("Voltage: "); Serial.println(voltage);
  Serial.print("Current: "); Serial.println(current);
  Serial.print("Power: "); Serial.println(power);

  delay(1000);
}
