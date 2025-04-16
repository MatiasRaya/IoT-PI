#pragma once

#include "utilities.h"
#include "logger/logger.h"

bool initGSM();
bool initGSM(const char* apn);

bool initWiFi();
bool initWiFi(const char* ssid, const char* psk);

bool initGPS();