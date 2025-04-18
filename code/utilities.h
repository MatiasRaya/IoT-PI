#pragma once

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (9)
#define MODEM_TX_PIN                        (11)
#define MODEM_RX_PIN                        (10)

#define BOARD_PWRKEY_PIN                    (18)
#define BOARD_LED_PIN                       (12)

#define BOARD_POWERON_PIN                   (BOARD_LED_PIN)
#define MODEM_RING_PIN                      (3)
#define MODEM_RESET_PIN                     (17)
#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1

#define BOARD_BAT_ADC_PIN                   (4)
#define BOARD_SOLAR_ADC_PIN                 (5)
#define BOARD_MISO_PIN                      (47)
#define BOARD_MOSI_PIN                      (14)
#define BOARD_SCK_PIN                       (21)
#define BOARD_SD_CS_PIN                     (13)

#ifndef TINY_GSM_MODEM_SIM7672
#define TINY_GSM_MODEM_SIM7672
#endif

#define MODEM_GPS_ENABLE_GPIO               (4)
#define MODEM_GPS_ENABLE_LEVEL              (1)
#define MODEM_GPS_MODE                      (3)
#define MODEM_GPS_OUTPUT_RATE               (1)
#define MODEM_GPS_CGA                       (1)
#define MODEM_GPS_GLL                       (1)
#define MODEM_GPS_GSA                       (1)
#define MODEM_GPS_GSV                       (1)
#define MODEM_GPS_RMC                       (1)
#define MODEM_GPS_VTG                       (1)
#define MODEM_GPS_ZDA                       (1)
#define MODEM_GPS_ANT                       (1)

#define I2C_SDA_PIN                         (1)
#define I2C_SCL_PIN                         (2)

#define LED_RED_PIN                         (42)
#define LED_GREEN_PIN                       (41)
#define LED_BLUE_PIN                        (40)
#define LED_PWR_ON                          LOW
#define LED_PWR_OFF                         HIGH

#define FLOW_SENSOR_PIN                     (15)

#define TINY_GSM_RX_BUFFER                  1024 // Set RX buffer to 1Kb

#define TINY_GSM_MODEM_SIM7672

#define NETWORK_APN                        "igprs.claro.com.ar" // Claro Argentina

#define NETWORK_SSID                       "iotPrueba"
#define NETWORK_PSK                        "123456789"

// // See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

#include "Arduino.h"

#include <TinyGsmClient.h>
#include <StreamDebugger.h>
#include <ArduinoHttpClient.h>
#include <TinyGPSPlus.h>
#include <stdarg.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <vector>
#include <algorithm>
#include <Wire.h>
#include <RTClib.h>