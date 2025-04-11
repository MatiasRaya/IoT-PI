#include "config.h"

#define classNAME "config"

Config readConfig() {
    Config config;

    if (!LittleFS.begin()) {
        // Serial.println("LittleFS mount failed");
        LOG_ERROR(classNAME, "LittleFS mount failed");
        return config;
    }

    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        // Serial.println("No se pudo abrir config.txt");
        LOG_ERROR(classNAME, "Could not open config.txt");
        return config;
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.startsWith("SN=")) {
            config.sn = line.substring(3);
        } else if (line.startsWith("APN=")) {
            config.apn = line.substring(4);
        } else if (line.startsWith("SSID=")) {
            config.ssid = line.substring(5);
        } else if (line.startsWith("PSK=")) {
            config.psk = line.substring(4);
        }else if (line.startsWith("NUMBER_FILES=")) {
            config.maxFiles = line.substring(13).toInt();
        } else if (line.startsWith("SIZE_FILES=")) {
            config.maxSizeMB = line.substring(11).toInt();
        }
    }

    file.close();

    if (config.maxFiles > 0 && config.maxSizeMB > 0) {
        setLogLimits(config.maxFiles, config.maxSizeMB);
        LOG_INFO(classNAME, "Log rotation config: %d files, %d MB each", config.maxFiles, config.maxSizeMB);
    }

    return config;
}
