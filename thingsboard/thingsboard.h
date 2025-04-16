#pragma once

#include "utilities.h"
#include "logger/logger.h"

struct Thingsboard {
    String url;
    String port;
    String deviceID;
    String username;
    String password;
};

void setData(Thingsboard &tb);
bool getToken();
void getDeviceData();
void postDeviceData();