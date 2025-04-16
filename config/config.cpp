#include "config.h"

#define classNAME "config"

Config readConfig() {
    Config config;

    if (!LittleFS.begin()) {
        LOG_ERROR(classNAME, "LittleFS mount failed");
        
        return config;
    }

    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        LOG_ERROR(classNAME, "Could not open config.txt");
        
        return config;
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.startsWith("SN=")) {
            config.sn = line.substring(3);
        }
        else if (line.startsWith("APN=")) {
            config.apn = line.substring(4);
        }
        else if (line.startsWith("SSID=")) {
            config.ssid = line.substring(5);
        }
        else if (line.startsWith("PSK=")) {
            config.psk = line.substring(4);
        }
        else if (line.startsWith("NUMBER_FILES=")) {
            config.maxFiles = line.substring(13).toInt();
        }
        else if (line.startsWith("SIZE_FILES=")) {
            config.maxSizeMB = line.substring(11).toInt();
        }
        else if (line.startsWith("URL=")) {
            config.url = line.substring(4);
        }
        else if (line.startsWith("PORT=")) {
            config.port = line.substring(5);
        }
        else if (line.startsWith("DEVICE_ID=")) {
            config.deviceID = line.substring(10);
        }
        else if (line.startsWith("USERNAME=")) {
            config.username = line.substring(9);
        }
        else if (line.startsWith("PASSWORD=")) {
            config.password = line.substring(9);
        }
    }

    file.close();

    if (config.maxFiles > 0 && config.maxSizeMB > 0) {
        setLogLimits(config.maxFiles, config.maxSizeMB);
        LOG_INFO(classNAME, "Log rotation config: %d files, %d MB each", config.maxFiles, config.maxSizeMB);
    }

    return config;
}
