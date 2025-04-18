#pragma once

#include "utilities.h"
#include "logger/logger.h"

struct Config {
    String sn;
    String apn;
    String ssid;
    String psk;
    int maxFiles = 5;
    int maxSizeMB = 1;
    String url = "";
    String port = "";
    String deviceID = "";
    String username = "";
    String password = "";
};

Config readConfig();