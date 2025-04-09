#include "network.h"

StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);

bool initGSM()
{
    bool retval = true;

    Serial.println("Start modem...");
    int retry = 0;

    while (!modem.testAT(1000)) {
        Serial.print(".");
        if (retry++ > 10)
        {
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_PWRKEY_PIN, HIGH);
            delay(1000);
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            retry = 0;
        }
    }
    Serial.println();
    
    Serial.println("Waiting for network...");
    
    SimStatus status = modem.getSimStatus();
    while (status != SIM_READY) {
        status = modem.getSimStatus();
 
        Serial.print(".");

        switch (status) {
            case SIM_LOCKED:
                Serial.println("SIM locked");
                retval = false;
                break;
            case SIM_READY:
                Serial.println("SIM ready");
                break;
            default:
                break;
        }

        delay(1000);
    }

    if (retval) {
        Serial.println("Network ready");

        modem.sendAT("+SIMCOMATI");
        modem.waitResponse();

        Serial.printf("Set network apn : %s\n", NETWORK_APN);
        modem.sendAT(GF("+CGDCONT=1,\"IP\",\""), NETWORK_APN, "\"");
        if (modem.waitResponse() != 1) {
            Serial.println("Set network apn error !");
        }

        int16_t sq ;
        Serial.println("Wait for the modem to register with the network.");
        RegStatus status = REG_NO_RESULT;
        while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED) {
            Serial.print(".");
            status = modem.getRegistrationStatus();

            switch (status) {
                case REG_UNREGISTERED:
                case REG_SEARCHING:
                    sq = modem.getSignalQuality();
                    Serial.printf("[%lu] Signal Quality: %d\n", millis() / 1000, sq);
                    delay(1000);
                    break;
                case REG_DENIED:
                    Serial.println("Network registration was rejected, please check if the APN is correct");
                    break;
                case REG_OK_HOME:
                    Serial.println("Online registration successful");
                    break;
                case REG_OK_ROAMING:
                    Serial.println("Network registration successful, currently in roaming mode");
                    break;
                default:
                    Serial.printf("Registration Status: %d\n", status);
                    Serial.println();
                    delay(1000);
                    break;
            }
        }


        Serial.printf("Registration Status: %d\n", status);
        delay(1000);

        String ueInfo;
        if (modem.getSystemInformation(ueInfo)) {
            Serial.print("Inquiring UE system information: ");
            Serial.println(ueInfo);
        }

        if (!modem.setNetworkActive()) {
            Serial.println("Enable network failed!");
            retval = false;
        }

        if (retval) {
            Serial.println("Network active");
            
            delay(5000);
    
            String ipAddress = modem.getLocalIP();
            Serial.print("Network IP: ");
            Serial.println(ipAddress);
            Serial.print("Network Signal Quality: ");
            Serial.println(modem.getSignalQuality());
    
            String resolved_ip_addr;
            uint32_t rep_data_packet_size;
            uint32_t tripTime;
            uint8_t TTL;
            for (int i = 0; i < 20; ++i) {
                int res = modem.ping("www.google.com", resolved_ip_addr, rep_data_packet_size, tripTime, TTL);
                if (res == 1) {
                    Serial.printf("Reply from %s: bytes=%u time=%ums TTL=%u\n", resolved_ip_addr, rep_data_packet_size, tripTime, TTL);
                    retval = true;
                } else {
                    Serial.printf("Error code : %d\n", res);
                    retval = false;
                }
            }
        } else {
            Serial.println("Network not active");
        }
    } else {
        Serial.println("Network not ready");
    }

    return retval;
}