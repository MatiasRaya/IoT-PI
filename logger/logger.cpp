#include "logger.h"

bool enableSDLogging = false;
bool enableRTCLogging = false;
int maxLogFiles = 5;
int maxFileSizeMB = 1;
String dateTime = "";

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

void enableSD(bool enable) {
    enableSDLogging = enable;
}

void enableRTC(bool enable) {
    enableRTCLogging = enable;
}

void setLogLimits(int maxFiles, int maxSizeMB) {
    maxLogFiles = maxFiles;
    maxFileSizeMB = maxSizeMB;
}

String formatLogFileName(time_t timestamp) {
    struct tm *tm_info = localtime(&timestamp);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "/log_%d%m%Y_%H%M%S.log", tm_info);
    return String(buffer);
}

void rotateLogsIfNeeded() {
    File logFile = SD.open("/log.txt");
    if (!logFile) return;

    if (logFile.size() > maxFileSizeMB * 1024 * 1024) {
        logFile.close();

        time_t now = time(nullptr);
        String newName = formatLogFileName(now);

        SD.rename("/log.txt", newName);

        File root = SD.open("/");
        std::vector<String> logFiles;

        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;

            String name = entry.name();
            if (name.startsWith("/log_") && name.endsWith(".log")) {
                logFiles.push_back(name);
            }
            entry.close();
        }

        root.close();
        
        if (logFiles.size() >= maxLogFiles) {
            std::sort(logFiles.begin(), logFiles.end());
            for (int i = 0; i <= logFiles.size() - maxLogFiles; ++i) {
                SD.remove(logFiles[i]);
            }
        }
    } else {
        logFile.close();
    }
}

void logf(LogLevel level, const char *tag, const char *func, const char *format, ...) {
    char msg[256];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    if (enableRTC) {
        dateTime = getRTCDateTime();
    } else {
        dateTime = "1990-01-01 00:00:00";
    }
    String finalMsg = String("[") + dateTime + "] [" + levelToString(level) + "] [" + tag + "] [" + func + "] " + msg + "\n";

    Serial.print(finalMsg);

    if (!enableSDLogging) {
        return;
    }

    rotateLogsIfNeeded();

    File logFile = SD.open("/log.txt", FILE_APPEND);
    if (logFile) {
        logFile.print(finalMsg);
        logFile.close();
    } else {
        Serial.println("ERROR: No se pudo escribir en log.txt");
    }
}