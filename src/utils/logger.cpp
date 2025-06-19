#include "logger.h"
#include <stdarg.h>

Logger::Logger() {
    _level = LOG_DEBUG;
    _defaultTag = "default";
}

void Logger::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial);
}

void Logger::setLogLevel(LogLevel level) {
    _level = level;
}

void Logger::setTag(const char* tag) {
    _defaultTag = tag;
}

const char* Logger::levelToString(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO ";
        case LOG_WARN:  return "WARN ";
        case LOG_ERROR: return "ERROR";
        default:        return "UNKWN";
    }
}

const char* Logger::levelToColor(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "\033[36m"; // Cyan
        case LOG_INFO:  return "\033[32m"; // Green
        case LOG_WARN:  return "\033[33m"; // Yellow
        case LOG_ERROR: return "\033[31m"; // Red
        default:        return "\033[0m";
    }
}

void Logger::_log(LogLevel level, const char* tag, const char* message) {
    if (level < _level) return;

    Serial.printf(
        "%s[%lu][%s][%s] %s\033[0m\n",
        levelToColor(level),
        millis(),
        levelToString(level),
        tag ? tag : _defaultTag,
        message
    );
}

void Logger::_logf(LogLevel level, const char* tag, const char* format, va_list args) {
    if (level < _level) return;

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);

    _log(level, tag, buffer);
}

void Logger::debug(const char* tag, const char* message) {
    _log(LOG_DEBUG, tag, message);
}

void Logger::info(const char* tag, const char* message) {
    _log(LOG_INFO, tag, message);
}

void Logger::warn(const char* tag, const char* message) {
    _log(LOG_WARN, tag, message);
}

void Logger::error(const char* tag, const char* message) {
    _log(LOG_ERROR, tag, message);
}

void Logger::debugf(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _logf(LOG_DEBUG, tag, format, args);
    va_end(args);
}

void Logger::infof(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _logf(LOG_INFO, tag, format, args);
    va_end(args);
}

void Logger::warnf(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _logf(LOG_WARN, tag, format, args);
    va_end(args);
}

void Logger::errorf(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _logf(LOG_ERROR, tag, format, args);
    va_end(args);
}
