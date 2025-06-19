#pragma once

#include <Arduino.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_NONE
};

class Logger {
public:
    Logger();
    void begin(unsigned long baudRate = 115200);
    void setLogLevel(LogLevel level);
    void setTag(const char* tag);

    void debug(const char* tag, const char* message);
    void info(const char* tag, const char* message);
    void warn(const char* tag, const char* message);
    void error(const char* tag, const char* message);

    void debugf(const char* tag, const char* format, ...);
    void infof(const char* tag, const char* format, ...);
    void warnf(const char* tag, const char* format, ...);
    void errorf(const char* tag, const char* format, ...);

private:
    LogLevel _level;
    const char* _defaultTag;

    const char* levelToString(LogLevel level);
    const char* levelToColor(LogLevel level);

    void _log(LogLevel level, const char* tag, const char* message);
    void _logf(LogLevel level, const char* tag, const char* format, va_list args);
};
