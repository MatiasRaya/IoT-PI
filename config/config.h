#pragma once

#include "utilities.h"
#include "logger/logger.h"

struct Config {
    String sn;
    String apn;
    String ssid;
    String psk;
};

Config readConfig();