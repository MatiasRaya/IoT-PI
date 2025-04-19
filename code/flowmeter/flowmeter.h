#pragma once

#include "utilities.h"
#include "logger/logger.h"

void initFlowmeter();
float getFlowRate();
float getTotalLiters();
void resetTotalLiters();