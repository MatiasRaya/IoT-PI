#include "logger.h"

bool enableSDLogging = false;

const char* levelToString(LogLevel level) {
    switch (level) {
        case LOG_INFO:
            return "INFO";
        case LOG_WARN: 
            return "WARN";
        case LOG_ERROR:
            return "ERROR";
        case LOG_DEBUG:
            return "DEBUG";
        default:       
            return "UNKNOWN";
    }
}

String getDateTime() {
    // TODO: Reemplazar por lectura real del RTC
    return "2025-04-09 22:15:00";
}

void enableSD(bool enable) {
    enableSDLogging = enable;
}

void logf(LogLevel level, const char *tag, const char *func, const char *format, ...) {
    char msg[256];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    String timestamp = getDateTime();
    String finalMsg = String("[") + timestamp + "] [" + levelToString(level) + "] [" + tag + "] [" + func + "] " + msg + "\n";

    Serial.print(finalMsg);


    if (!enableSDLogging) {
        return;
    }

    File logFile = SD.open("/log.txt", FILE_APPEND);
    if (logFile) {
        logFile.print(finalMsg);
        logFile.close();
    } else {
        Serial.println("ERROR: No se pudo escribir en log.txt");
    }
}