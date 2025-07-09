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
        else if (line.startsWith("LONGITUDE=")) {
            config.longitude = line.substring(10).toFloat();
        }
        else if (line.startsWith("LATITUDE=")) {
            config.latitude = line.substring(9).toFloat();
        }
        else if (line.startsWith("SSID=")) {
            config.ssid = line.substring(5);
        }
        else if (line.startsWith("PSK=")) {
            config.psk = line.substring(4);
        }
        else if (line.startsWith("flowMeter=")) {
            config.flowMeter = line.substring(11).toInt();
        }
        else if (line.startsWith("flowMeterTotal=")) {
            config.flowMeterTotal = line.substring(17).toInt();
        }
        else if (line.startsWith("isWIFI=")) {
            config.isWiFi = (line.substring(8) == "true");
        }
        else if (line.startsWith("isGPRS=")) {
            config.isGPRS = (line.substring(8) == "true");
        }
        else if (line.startsWith("isToken=")) {
            config.isToken = (line.substring(9) == "true");
        }
        else if (line.startsWith("URL=")) {
            config.url = line.substring(4);
        }
        else if (line.startsWith("CLAVE_APROVISIONAMIENTO=")) {
            config.claveAprovisionamiento = line.substring(23);
        }
        else if (line.startsWith("SECRETO_APROVISIONAMIENTO=")) {
            config.secretoAprovisionamiento = line.substring(26);
        }
        else if (line.startsWith("TOKEN=")) {
            config.token = line.substring(6);
        }
    }

    file.close();

    return config;
}

void readAllConfig() {
    if (!LittleFS.begin()) {
        LOG_ERROR(classNAME, "LittleFS mount failed");
    }

    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        LOG_ERROR(classNAME, "Could not open config.txt");
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();

        LOG_DEBUG(classNAME, "%s", line.c_str());
    }
}

bool updateConfigField(const String& key, const String& newValue) {
    if (!LittleFS.exists("/config.txt")) {
        LOG_ERROR(classNAME, "config.txt not found");
        LittleFS.remove("/config.txt");

        return false;
    }

    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        LOG_ERROR(classNAME, "Could not open config.txt for reading");

        return false;
    }

    String newContent;
    bool keyFound = false;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.startsWith(key + "=")) {
            newContent += key + "=" + newValue + "\n";
            keyFound = true;
        } else {
            newContent += line + "\n";
        }
    }
    file.close();
    
    if (!keyFound) {
        newContent += key + "=" + newValue + "\n";
    }
    
    file = LittleFS.open("/config.txt", "w");
    if (!file) {
        LOG_ERROR(classNAME, "Could not open config.txt for writing");
        return false;
    }

    file.print(newContent);
    file.close();

    LOG_DEBUG(classNAME, "Updated '%s' = '%s'", key.c_str(), newValue.c_str());
    return true;
}

bool updateConfigField(const String& key, int value) {
    return updateConfigField(key, String(value));
}

bool updateConfigField(const String& key, float value) {
    return updateConfigField(key, String(value, 6));
}

bool updateConfigField(const String& key, bool value) {
    LOG_DEBUG(classNAME, "Update %s", key);

    String valueSTR = value ? "true" : "false";

    return updateConfigField(key, valueSTR);
}
