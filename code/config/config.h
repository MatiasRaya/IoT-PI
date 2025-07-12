#pragma once

#include "utilities.h"
#include "logger/logger.h"

struct Config {
    String sn;
    String apn;
    float longitude;
    float latitude;
    String ssid;
    String psk;
    int flowMeter;
    int flowMeterTotal;
    bool isWiFi;
    bool isGPRS;
    bool isToken;
    String url_thingsboard;
    String claveAprovisionamiento;
    String secretoAprovisionamiento;
    String token;
    String url_macs;
};

Config readConfig();
void readAllConfig();
bool updateConfigField(const String& key, const String& newValue);
bool updateConfigField(const String& key, int value);
bool updateConfigField(const String& key, float value);
bool updateConfigField(const String& key, bool value);