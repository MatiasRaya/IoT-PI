#include "network/network.h"
#include "logger/logger.h"
#include "sd/sd.h"
#include "config/config.h"
#include "rtc/rtc.h"
#include "thingsboard/thingsboard.h"
#include "led/led.h"
#include "flowmeter/flowmeter.h"
#include "utilities.h"

#define classNAME "main"

bool enableGSM = true, enableWiFi = true, enableGPS = true;
float latitude = 0.0f, longitude = 0.0f;

Config cfg;

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

    initFlowmeter();

    initLed();

    setLedState(LED_OFF, LED_COLOR_GREEN);
    setLedState(LED_OFF, LED_COLOR_RED);
    setLedState(LED_OFF, LED_COLOR_YELLOW);

    setLedState(LED_ON, LED_COLOR_BLUE);

    LOG_INFO(classNAME, "SD initialized successfully");

    if (!LittleFS.begin()) {
        LOG_ERROR(classNAME, "Failed to mount LittleFS");
    } else {
        LOG_INFO(classNAME, "LittleFS mounted successfully");
    }

    cfg = readConfig();
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

    if (cfg.apn.length() > 0) {
        LOG_INFO(classNAME, "APN configured, using custom APN");

        enableGSM = initGSM(cfg.apn.c_str());
    } else {
        LOG_INFO(classNAME, "APN not configured, using default APN");

        enableGSM = initGSM();
    }

    enableGPS = initGPS();

    if (enableGPS) {
        LOG_INFO(classNAME, "GPS initialized successfully");
    } else {
        LOG_ERROR(classNAME, "GPS initialization failed");

        enableGPS = false;
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

    if (enableGPS) {
        if (getGPSLocation(latitude, longitude)) {
            LOG_INFO(classNAME, "GPS location: %f, %f", latitude, longitude);

            setLedState(LED_ON, LED_COLOR_GREEN);
        } else {
            LOG_ERROR(classNAME, "Failed to get GPS location");

            setLedState(LED_ON, LED_COLOR_RED);
        }
    } else {
        LOG_ERROR(classNAME, "GPS not enabled");

        setLedState(LED_ON, LED_COLOR_RED);
    }
}

void loop()
{
    if (enableGPS) {
        if (getGPSLocation(latitude, longitude)) {
            LOG_INFO(classNAME, "GPS location: %f, %f", latitude, longitude);
        } else {
            LOG_ERROR(classNAME, "Failed to get GPS location");
        }
    }

    static unsigned long lastFlowLog = 0;

    if (millis() - lastFlowLog > 1000) {
        float flowRate = getFlowRate();
        if (flowRate >= 0.0f) {
            LOG_INFO(classNAME, "Flow rate: %.2f L/min", flowRate);
            LOG_INFO(classNAME, "Total liters: %.2f L", getTotalLiters());
        } else {
            LOG_ERROR(classNAME, "Failed to get flow rate");
        }

        lastFlowLog = millis();
    }

    if (enableGSM || enableWiFi) {
        if (enableWiFi) {
            LOG_INFO(classNAME, "WiFi enabled, posting data to Thingsboard");
        } else {
            LOG_INFO(classNAME, "GSM enabled, posting data to Thingsboard");
        }

        postDeviceData("SN", cfg.sn);
        postDeviceData("apn", cfg.apn);
        postDeviceData("ssid", cfg.ssid);
        postDeviceData("psk", cfg.psk);

        postDeviceData("enableGSM", enableGSM);
        postDeviceData("enableWiFi", enableWiFi);

        if (enableGPS) {
            postDeviceData("latitude", latitude);
            postDeviceData("longitude", longitude);
        }

        postDeviceData("flow_rate", getFlowRate());
        postDeviceData("total_liters", getTotalLiters());
    } else {
        LOG_ERROR(classNAME, "GSM and WiFi not enabled, cannot post data to Thingsboard");
    }

    delay(1000);
}
