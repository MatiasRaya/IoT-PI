#include "network/network.h"
#include "logger/logger.h"
#include "sd/sd.h"
#include "config/config.h"
#include "rtc/rtc.h"
#include "thingsboard/thingsboard.h"
#include "utilities.h"

#define classNAME "main"

void setup()
{
    Serial.begin(MODEM_BAUDRATE);

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
        enableRTC(false);
        enableSD(false);

        Serial.println("SD initialized failed");
        // LOG_ERROR(classNAME, "SD initialization failed");
    }
    else {
        enableRTC(false);
        enableSD(true);

        Serial.println("SD initialized successfully");
        // LOG_INFO(classNAME, "SD initialized successfully");
    }

    if(!initRTC()) {
        enableRTC(false);

        LOG_ERROR(classNAME, "RTC initialization failed");
    } else {
        LOG_INFO(classNAME, "RTC initialized successfully");

        enableRTC(true);

        String dateTime = getRTCDateTime();
        LOG_INFO(classNAME, "RTC DateTime: %s", dateTime.c_str());
    }

    LOG_INFO(classNAME, "SD initialized successfully");

    if (!LittleFS.begin()) {
        LOG_ERROR(classNAME, "Failed to mount LittleFS");
    } else {
        LOG_INFO(classNAME, "LittleFS mounted successfully");
    }

    Config cfg = readConfig();
    LOG_INFO(classNAME, "SN: %s", cfg.sn.c_str());
    LOG_INFO(classNAME, "APN: %s", cfg.apn.c_str());
    LOG_INFO(classNAME, "SSID: %s, PSK: %s", cfg.ssid.c_str(), cfg.psk.c_str());
    LOG_INFO(classNAME, "Max files: %d, Max size: %d MB", cfg.maxFiles, cfg.maxSizeMB);
    LOG_INFO(classNAME, "URL: %s, Port: %s", cfg.url.c_str(), cfg.port.c_str());
    LOG_INFO(classNAME, "Device ID: %s", cfg.deviceID.c_str());
    LOG_INFO(classNAME, "Username: %s, Password: %s", cfg.username.c_str(), cfg.password.c_str());
    LOG_INFO(classNAME, "Config read successfully");

    Thingsboard tb;
    tb.url = cfg.url;
    tb.port = cfg.port;
    tb.deviceID = cfg.deviceID;
    tb.username = cfg.username;
    tb.password = cfg.password;
    setData(tb);
    LOG_INFO(classNAME, "Thingsboard data set successfully");

    bool enableGSM = true, enableWiFi = true;

    if (cfg.apn.length() > 0) {
        LOG_INFO(classNAME, "APN configured, using custom APN");

        enableGSM = initGSM(cfg.apn.c_str());
    } else {
        LOG_INFO(classNAME, "APN not configured, using default APN");

        enableGSM = initGSM();
    }

    if (enableGSM) {
        LOG_INFO(classNAME, "GSM initialized successfully");
    } else {
        LOG_ERROR(classNAME, "GSM initialization failed");

        enableGSM = false;

        enableWiFi = false;

        if (cfg.ssid.length() > 0 && cfg.psk.length() > 0) {

            LOG_INFO(classNAME, "WiFi configured, using custom SSID and PSK");

            enableWiFi = initWiFi(cfg.ssid.c_str(), cfg.psk.c_str());
        } else {

            LOG_INFO(classNAME, "WiFi not configured, using default SSID and PSK");

            enableWiFi = initWiFi();
        }

        if (enableWiFi) {

            LOG_INFO(classNAME, "WiFi initialized successfully");
        } else {

            LOG_ERROR(classNAME, "WiFi initialization failed");
        }
    }

    if (getToken()) {
        LOG_INFO(classNAME, "Token received successfully");

        getDeviceData();
    } else {
        LOG_ERROR(classNAME, "Failed to get token from Thingsboard");
    }
}

void loop()
{
    delay(1000);
}
