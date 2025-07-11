#include "rtc.h"

#define classNAME "rtc"

RTC_DS3231 rtc;

bool initRTC() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    if (!rtc.begin()) {
        LOG_ERROR(classNAME, "Couldn't find RTC");
        return false;
    }

    return true;
}

String getRTCDateTime() {
    char buffer[20];
    DateTime now = rtc.now();
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             now.year(), now.month(), now.day(),
             now.hour(), now.minute(), now.second());
    return String(buffer);
}

DateTime getRTC() {
    return rtc.now();
}

void setRTCDateTime(time_t t) {
    struct tm *timeinfo = localtime(&t);
    rtc.adjust(DateTime(timeinfo->tm_year + 1900,
                        timeinfo->tm_mon + 1,
                        timeinfo->tm_mday,
                        timeinfo->tm_hour,
                        timeinfo->tm_min,
                        timeinfo->tm_sec));

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    LOG_DEBUG(classNAME, "RTC DateTime set to: %s", buffer);
}