#pragma once

#include "utilities.h"
#include "logger/logger.h"
#include "config/config.h"

struct Thingsboard {
    String url;
    String sn;
    String token;
    String secretProvisioning;
    String keyProvisioning;
};

struct MACs {
    String url;
};

bool initGSM();
bool initGSM(const char* apn);

bool initWiFi();
bool initWiFi(const char* ssid, const char* psk);

bool initGPS();
bool getGPSLocation(float& latitude, float& longitude);

void setData(Thingsboard &tb, MACs &mac);

void getToken();
bool postData(const String& key, const String& newValue);
bool postMac(const String& key, const String& newValue);
bool getUpdateAllData();
bool getUpdateData(const String& key);
bool getMac();

void syncRTCESP32();

void sendMAC();