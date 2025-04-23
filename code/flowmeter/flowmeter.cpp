#include "flowmeter.h"

#define classNAME "flowmeter"

volatile int pulseCount = 0;
unsigned long currentTime;
unsigned long cloopTime;
float vol = 0.0;
float l_minute = 0.0;
const uint8_t flowsensor = 15;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR pulseCounter() {
    portENTER_CRITICAL_ISR(&mux);
    pulseCount++;
    portEXIT_CRITICAL_ISR(&mux);
}

void initFlowmeter() {
    // pinMode(flowsensor, INPUT_PULLUP);
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    
    // attachInterrupt(flowsensor, pulseCounter, RISING);
    attachInterrupt(FLOW_SENSOR_PIN, pulseCounter, RISING);
    
    currentTime = millis();
    cloopTime = currentTime;
    
    // LOG_INFO(classNAME, "Flowmeter initialized on pin %d", flowsensor);
    LOG_INFO(classNAME, "Flowmeter initialized on pin %d", FLOW_SENSOR_PIN);
}

void getFlowRate() {
    currentTime = millis();

    if (currentTime - cloopTime >= 1000) {
        cloopTime = currentTime;

        int freq;
        portENTER_CRITICAL(&mux);
        freq = pulseCount;
        pulseCount = 0;
        portEXIT_CRITICAL(&mux);

        if (freq != 0) {
            l_minute = freq / 7.5;
        } else {
            l_minute = 0;
        }

        vol += (l_minute / 60.0);

        LOG_INFO(classNAME, "Flow rate: %.2f L/min", l_minute);
        LOG_INFO(classNAME, "Volume: %.2f mL", vol*1000.0);
        LOG_INFO(classNAME, "Volume: %.2f L", vol);
    }
}

// float getTotalLiters() {
//     LOG_INFO(classNAME, "Total litres: %.2f L", totalMilliLitres / 1000.0);
//     return totalMilliLitres / 1000.0;
// }

// float getTotalMilliLitres() {
//     LOG_INFO(classNAME, "Total milliLitres: %lu ml", totalMilliLitres);
//     return totalMilliLitres;
// }

// uint32_t getPulseCount() {
//     LOG_INFO(classNAME, "Pulse count: %lu", pulseCount);
//     return pulseCount;
// }
