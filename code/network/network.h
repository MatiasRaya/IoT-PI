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

bool initGSM();
bool initGSM(const char* apn);

bool initWiFi();
bool initWiFi(const char* ssid, const char* psk);

bool initGPS();
bool getGPSLocation(float& latitude, float& longitude);

void setData(Thingsboard &tb);

void getToken();
bool postData(const String& key, const String& newValue);
bool getUpdateAllData();
bool getUpdateData(const String& key);

// void getDeviceData(String data);
// void postDeviceData(String key, int value);
// void postDeviceData(String key, float value);
// void postDeviceData(String key, bool value);

void syncRTCESP32();