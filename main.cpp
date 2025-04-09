#include "network/network.h"
#include "SD/sd.h"

void setup()
{
    Serial.begin(MODEM_BAUDRATE);

    Serial.println("Start Sketch");

    SerialAT.begin(MODEM_BAUDRATE, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // initGSM("datos.personal.com");
    if (initGSM()) {
        Serial.println("GSM initialized successfully");
    } else {
        Serial.println("GSM initialization failed");
    }
}

void loop()
{
}
