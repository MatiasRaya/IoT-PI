#pragma once

#include "utilities.h"
#include "logger/logger.h"

struct Thingsboard {
    String url;
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
bool getToken();
void getDeviceData(String data);
void postDeviceData(String key, int value);
void postDeviceData(String key, float value);
void postDeviceData(String key, String value);
void postDeviceData(String key, bool value);

void syncRTCESP32();