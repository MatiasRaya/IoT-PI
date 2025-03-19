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

#define TINY_GSM_MODEM_SIM7600

#define MODEM_GPS_ENABLE_GPIO               (4)
#define MODEM_GPS_ENABLE_LEVEL              (1)

#include "Arduino.h"
#include "FS.h"
#include "LittleFS.h"

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
