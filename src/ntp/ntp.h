#ifndef NTP_H
#define NTP_H

#include <time.h>
#include <Arduino.h>

void ntpInit();
String getTimestamp();

#endif