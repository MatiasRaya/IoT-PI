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

#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

#define TINY_GSM_MODEM_SIM7672

#define NETWORK_APN     "igprs.claro.com.ar"             // Claro Argentina

// // See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

#include "Arduino.h"

#include <TinyGsmClient.h>
#include <StreamDebugger.h>
#include <ArduinoHttpClient.h>

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "igprs.claro.com.ar"             //CHN-CT: China Telecom
// #define NETWORK_APN     "datos.personal.com"             //CHN-CT: China Telecom
