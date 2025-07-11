#include "led.h"

#define classNAME "led"

void initLed() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    digitalWrite(LED_RED_PIN, LED_PWR_OFF);
    digitalWrite(LED_GREEN_PIN, LED_PWR_OFF);
    digitalWrite(LED_BLUE_PIN, LED_PWR_OFF);
}

void setLedState(LedState state, LedColor color) {
    switch (color) {
        case LED_COLOR_RED:
            digitalWrite(LED_RED_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_GREEN_PIN, LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Red %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_GREEN:
            digitalWrite(LED_GREEN_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_RED_PIN, LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Green %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_BLUE:
            digitalWrite(LED_BLUE_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_RED_PIN, LED_PWR_OFF);
            digitalWrite(LED_GREEN_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Blue %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_YELLOW:
            digitalWrite(LED_RED_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_GREEN_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Yellow %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_CYAN:
            digitalWrite(LED_GREEN_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_RED_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Cyan %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_MAGENTA:
            digitalWrite(LED_RED_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_GREEN_PIN, LED_PWR_OFF);
            LOG_INFO(classNAME, "LED Magenta %s", state == LED_ON ? "ON" : "OFF");
            break;
        case LED_COLOR_WHITE:
            digitalWrite(LED_RED_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_GREEN_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            digitalWrite(LED_BLUE_PIN, state == LED_ON ? LED_PWR_ON : LED_PWR_OFF);
            LOG_INFO(classNAME, "LED White %s", state == LED_ON ? "ON" : "OFF");
            break;
        default:
            break;
    }
}