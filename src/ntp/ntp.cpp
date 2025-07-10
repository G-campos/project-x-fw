#include "ntp.h"

#include "config.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

struct tm timeinfo;

String getTimestamp() {
  time_t now;

  time(&now);
  localtime_r(&now, &timeinfo);

  char buffer[21];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y,%H:%M:%S", &timeinfo);

  return String(buffer);
}

void ntpInit() {
  const char* TAG = "ntpInit";

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  logger.info(TAG, getTimestamp().c_str());
}
