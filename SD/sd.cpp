#include "sd.h"

bool initSD() {
    SPI.begin(BOARD_SCK_PIN, BOARD_MISO_PIN, BOARD_MOSI_PIN);
    if (!SD.begin(BOARD_SD_CS_PIN)) {
        Serial.println("ERROR: No se pudo montar la SD");
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("ERROR: No se detectó una SD");
        return false;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SDSC");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("UNKNOWN");

    Serial.printf("Tamaño: %lluMB\n", SD.cardSize() / (1024 * 1024));
    return true;
}