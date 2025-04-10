#include "network/network.h"
#include "logger/logger.h"
#include "sd/sd.h"
#include "config/config.h"
#include "utilities.h"

#define classNAME "main"

void setup()
{
    Serial.begin(MODEM_BAUDRATE);

    // Serial.println("Start Sketch");

    SerialAT.begin(MODEM_BAUDRATE, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    if (!initSD()) {
        Serial.println("SD initialized failed");

        enableSD(false);
    }
    else {
        Serial.println("SD initialized successfully");
        enableSD(true);
    }

    LOG_INFO(classNAME, "SD initialized successfully");

    if (!LittleFS.begin()) {
        // Serial.println("Error al montar LittleFS");
        LOG_ERROR(classNAME, "Failed to mount LittleFS");
    } else {
        LOG_INFO(classNAME, "LittleFS mounted successfully");
    }

    Config cfg = readConfig();
    LOG_INFO(classNAME, "SN: %s, APN: %s, SSID: %s", cfg.sn.c_str(), cfg.apn.c_str(), cfg.ssid.c_str());

    bool enableGSM = true, enableWiFi = true;

    if (cfg.apn.length() > 0) {
        // Serial.println("APN configured, using custom APN");
        LOG_INFO(classNAME, "APN configured, using custom APN");

        enableGSM = initGSM(cfg.apn.c_str());
    } else {
        // Serial.println("APN not configured, using default APN");
        LOG_INFO(classNAME, "APN not configured, using default APN");

        enableGSM = initGSM();
    }

    if (enableGSM) {
        // Serial.println("GSM initialized successfully");
        LOG_INFO(classNAME, "GSM initialized successfully");
    } else {
        // Serial.println("GSM initialization failed");
        LOG_ERROR(classNAME, "GSM initialization failed");

        enableGSM = false;

        enableWiFi = initWiFi(cfg.ssid.c_str(), cfg.psk.c_str());

        if (cfg.ssid.length() > 0 && cfg.psk.length() > 0) {
            // Serial.println("WiFi configured, using custom SSID and PSK");
            LOG_INFO(classNAME, "WiFi configured, using custom SSID and PSK");

            enableWiFi = initWiFi(cfg.ssid.c_str(), cfg.psk.c_str());
        } else {
            // Serial.println("WiFi not configured, using default SSID and PSK");
            LOG_INFO(classNAME, "WiFi not configured, using default SSID and PSK");

            enableWiFi = initWiFi();
        }

        if (enableWiFi) {
            // Serial.println("WiFi initialized successfully");
            LOG_INFO(classNAME, "WiFi initialized successfully");
        } else {
            // Serial.println("WiFi initialization failed");
            LOG_ERROR(classNAME, "WiFi initialization failed");
        }
    }
}

void loop()
{
}
