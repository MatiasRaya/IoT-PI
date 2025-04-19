#include "flowmeter.h"

#define classNAME "flowmeter"

volatile uint32_t pulseCount = 0;
float totalLiters = 0.0f;

void IRAM_ATTR pulseCounter() {
    pulseCount++;
}

void initFlowmeter() {
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
    
    totalLiters = 0.0f;
    pulseCount = 0;
    
    LOG_INFO(classNAME, "Flowmeter initialized on pin %d", FLOW_SENSOR_PIN);
}

float getFlowRate() {
    static uint32_t lastPulseCount = 0;
    static unsigned long lastTime = 0;

    unsigned long currentTime = millis();

    if (currentTime - lastTime < 1000) {
        LOG_ERROR(classNAME, "getFlowRate: Not enough time elapsed since last call");

        return -1.0f;
    }

    noInterrupts();
    uint32_t currentPulseCount = pulseCount;
    interrupts();

    uint32_t pulsesInInternal = currentPulseCount - lastPulseCount;
    lastPulseCount = currentPulseCount;
    lastTime = currentTime;

    float flowRate = (pulsesInInternal / 7.5f);
    totalLiters += (flowRate / 60.0f);

    LOG_INFO(classNAME, "Flow rate: %.2f L/min, Total liters: %.2f L", flowRate, totalLiters);

    return flowRate;
}

float getTotalLiters() {
    return totalLiters;
}

void resetTotalLiters() {
    totalLiters = 0.0f;
    LOG_INFO(classNAME, "Total liters reset to 0.0 L");
}