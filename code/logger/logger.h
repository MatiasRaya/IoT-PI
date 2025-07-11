#pragma once

#include "rtc/rtc.h"
#include "utilities.h"

enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
};

void logf(LogLevel level, const char *tag, const char *func, const char *format, ...);
void enableSD(bool enable);
void enableRTC(bool enable);
void setLogLimits(int maxFiles, int maxSizeMB);
void enableSemaphore();

#define LOG_INFO(tag, fmt, ...)  logf(LOG_INFO, tag, __func__, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  logf(LOG_WARN, tag, __func__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(tag, fmt, ...) logf(LOG_ERROR, tag, __func__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(tag, fmt, ...) logf(LOG_DEBUG, tag, __func__, fmt, ##__VA_ARGS__)