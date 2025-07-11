#pragma once

#include "utilities.h"
#include "logger/logger.h"

bool initRTC();
String getRTCDateTime();
DateTime getRTC();
void setRTCDateTime(time_t t);