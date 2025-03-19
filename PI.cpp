#include "gsm/gsm.h"

void setup()
{
    Serial.begin(115200);
    
    Serial.println("Start Sketch");

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
    
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

#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    delay(5000);

    if (!LittleFS.begin(true)) {
        Serial.println("Error al montar LittleFS");
        return;
    }

    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        Serial.println("Error al abrir el archivo");
        return;
    }

    String apnName = "";
    String serialNumber = "";

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.startsWith("apn="))
        {
            apnName = line.substring(4);
        }
        
        if (line.startsWith("serial="))
        {
            serialNumber = line.substring(7);
        }
    }
    file.close();

    Serial.println("APN: " + apnName);
    Serial.println("Número de serie: " + serialNumber);

    if (apnName == "" || serialNumber == "")
    {
        Serial.println("Error al leer el archivo de configuración");
        return;
    }
    else
    {
        if (checkSimCard())
        {
            if (connetToNetwork(apnName.c_str()))
            {
                String response = sendATCommand("+SIMCOMATI", 5000);

                Serial.println("Respuesta recibida: " + response);

                if (response != "" && response.indexOf("OK") != -1)
                {
                    delay(10000);
                    
                    Serial.println("IP:" + getLocalIP());

                    Serial.println("Signal: " + getSignalQuality());

                    if (sendHttpGetRequest())
                    {
                        Serial.println("Todas las solicitudes se realizaron correctamente y se usa GSM");
                    }
                    else
                    {
                        Serial.println("Error al realizar las solicitudes");
                    }
                }
                else
                {
                    Serial.println("Error en la respuesta");
                }
            }
        }
    }
}

void loop()
{

}
