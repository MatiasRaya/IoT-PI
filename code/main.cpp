#include "utilities.h"
#include "network/network.h"
#include "logger/logger.h"
#include "sd/sd.h"
#include "config/config.h"
#include "rtc/rtc.h"
#include "led/led.h"
#include "flowmeter/flowmeter.h"

#define classNAME "main"

bool enableGSM = false, enableWiFi = false, enableGPS = false;
float latitude = 0.0f, longitude = 0.0f;
float volML = 0.0f;
unsigned long lastRTCSync = 0;

Config cfg;

// TaskHandle_t flowmeterTask = NULL;

// void flowmeterTaskFunction(void *pvParameters)
// {
//     initFlowmeter();

//     while (true) {
//         getFlowRate();

//         volML = getTotalMilliLitres();
//         LOG_INFO(classNAME, "millilitres: %.2f", volML);

//         delay(1000);
//     }
// }

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

    enableSemaphore();

    if(!initRTC()) {
        enableRTC(false);

        LOG_ERROR(classNAME, "RTC initialization failed");
    } else {
        LOG_INFO(classNAME, "RTC initialized successfully");

        enableRTC(true);

        String dateTime = getRTCDateTime();
        LOG_INFO(classNAME, "RTC DateTime: %s", dateTime.c_str());
    }

    // xTaskCreatePinnedToCore(
    //     flowmeterTaskFunction,
    //     "Flowmeter Task",
    //     10000,
    //     NULL,
    //     1,
    //     &flowmeterTask,
    //     0
    // );

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

    readAllConfig();

    LOG_INFO(classNAME, "SN: %s", cfg.sn.c_str());
    LOG_INFO(classNAME, "APN: %s", cfg.apn.c_str());
    LOG_INFO(classNAME, "SSID: %s, PSK: %s", cfg.ssid.c_str(), cfg.psk.c_str());
    LOG_INFO(classNAME, "URL_THINGSBOARD: %s", cfg.url_thingsboard.c_str());
    LOG_INFO(classNAME, "Key Provisioning: %s", cfg.claveAprovisionamiento.c_str());
    LOG_INFO(classNAME, "Secret Provisioning: %s", cfg.secretoAprovisionamiento.c_str());
    LOG_INFO(classNAME, "URL_MACS: %s", cfg.url_macs.c_str());
    LOG_INFO(classNAME, "Config read successfully");

    Thingsboard tb;
    tb.url = cfg.url_thingsboard.c_str();
    tb.token = cfg.token.c_str();
    tb.sn = cfg.sn.c_str();
    tb.secretProvisioning = cfg.secretoAprovisionamiento.c_str();
    tb.keyProvisioning = cfg.claveAprovisionamiento.c_str();
    
    MACs macs;
    macs.url = cfg.url_macs.c_str();
    
    setData(tb, macs);
    LOG_INFO(classNAME, "Thingsboard and MAcs data set successfully");
    
    delay(1000);

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

        enableGPS = true;
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

    if (enableGSM || enableWiFi) {
        syncRTCESP32();

        sendMAC();

        postData("SN", cfg.sn);
        postData("isWiFi", (cfg.isWiFi ? "true" : "false"));
        postData("isGPRS", (cfg.isGPRS ? "true" : "false"));
        postData("SSID", cfg.ssid);
        postData("PSWD", cfg.psk);
        // postData("APN", cfg.apn);

        // getUpdateAllData();

        getUpdateData("APN");
    }
    else {
        LOG_ERROR(classNAME, "No internet available for NTP sync");
    }

    if (enableGSM) {
        updateConfigField("isGPRS", true);
    }
    else {
        updateConfigField("isGPRS", false);
    }

    if (enableWiFi) {
        updateConfigField("isWiFi", true);
    }
    else {
        updateConfigField("isWiFi", false);
    }

    cfg = readConfig();

    if (cfg.token == "null") {
        getToken();
    }

    LOG_DEBUG(classNAME, "cfg.isWiFi: %s", (cfg.isWiFi ? "true" : "false"));
}

void loop()
{
    // getGPSLocation(latitude, longitude);

    static unsigned long lastRTCSync = 0;
    unsigned long currentMillis = millis();

    if (currentMillis - lastRTCSync >= 3600000) {
        if (enableGSM || enableWiFi) {
            LOG_DEBUG(classNAME, "Checking RTC sync with NTP (every hour)");

            syncRTCESP32();
        } else {
            LOG_ERROR(classNAME, "No internet available for NTP sync");
        }

        lastRTCSync = currentMillis;
    }

    delay(1000);
}
