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
        }
    }

    file.close();
    return config;
}
