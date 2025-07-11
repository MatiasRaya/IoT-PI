#pragma once

#include "utilities.h"
#include "logger/logger.h"

enum LedState {
    LED_OFF,
    LED_ON,
    LED_BLINKING
};

enum LedColor {
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_YELLOW,
    LED_COLOR_CYAN,
    LED_COLOR_MAGENTA,
    LED_COLOR_WHITE
};

void initLed();
void setLedState(LedState state, LedColor color);