#include "network.h"

#define classNAME "network"

StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);

bool initGSM()
{
    return initGSM(NETWORK_APN);
}

bool initGSM(const char *apn)
{
    bool retval = true;

    // Serial.println("Start modem...");
    LOG_INFO(classNAME, "Start modem...");
    int retry = 0;

    while (!modem.testAT(1000)) {
        // Serial.print(".");
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
    
    // Serial.println("Waiting for network...");
    LOG_INFO(classNAME, "Waiting for network...");
    
    SimStatus status = modem.getSimStatus();
    int simRetries = 0;
    while (status != SIM_READY && simRetries < 5) {
        status = modem.getSimStatus();
        // Serial.print(".");
        LOG_INFO(classNAME, "SIM status: %d", status);
        LOG_INFO(classNAME, "SIM retries: %d", simRetries);
        simRetries++;

        switch (status) {
            case SIM_LOCKED:
                // Serial.println("SIM locked");
                LOG_ERROR(classNAME, "SIM locked");
                retval = false;
                break;
            case SIM_READY:
                // Serial.println("SIM ready");
                LOG_INFO(classNAME, "SIM ready");
                break;
            case SIM_ERROR:
            default:
                    LOG_ERROR(classNAME, "SIM not inserted or unknown SIM status: %d", status);
                    retval = false;
                    break;
        }

        delay(1000);
    }

    if (retval) {
        // Serial.println("Network ready");
        LOG_INFO(classNAME, "Network ready");

        modem.sendAT("+SIMCOMATI");
        modem.waitResponse();

        Serial.printf("Set network apn : %s\n", apn);
        modem.sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, "\"");
        if (modem.waitResponse() != 1) {
            // Serial.println("Set network apn error !");
            LOG_ERROR(classNAME, "Set network apn error!");
            retval = false;
        }

        if (!retval) {
            // Serial.println("Network not ready");
            LOG_ERROR(classNAME, "Network not ready");
            return false;
        }

        int16_t sq ;
        // Serial.println("Wait for the modem to register with the network.");
        LOG_INFO(classNAME, "Wait for the modem to register with the network.");
        RegStatus status = REG_NO_RESULT;
        while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED) {
            Serial.print(".");
            status = modem.getRegistrationStatus();

            switch (status) {
                case REG_UNREGISTERED:
                case REG_SEARCHING:
                    sq = modem.getSignalQuality();
                    // Serial.printf("[%lu] Signal Quality: %d\n", millis() / 1000, sq);
                    LOG_INFO(classNAME, "Signal Quality: %d", sq);
                    delay(1000);
                    break;
                case REG_DENIED:
                    // Serial.println("Network registration was rejected, please check if the APN is correct");
                    LOG_ERROR(classNAME, "Network registration was rejected, please check if the APN is correct");
                    retval = false;
                    break;
                case REG_OK_HOME:
                    // Serial.println("Online registration successful");
                    LOG_INFO(classNAME, "Online registration successful");
                    break;
                case REG_OK_ROAMING:
                    // Serial.println("Network registration successful, currently in roaming mode");
                    LOG_INFO(classNAME, "Network registration successful, currently in roaming mode");
                    break;
                default:
                    Serial.printf("Registration Status: %d\n", status);
                    Serial.println();
                    delay(1000);
                    break;
            }
        }

        if (!retval) {
            // Serial.println("Network not ready");
            LOG_ERROR(classNAME, "Network not ready");
            return false;
        }

        // Serial.printf("Registration Status: %d\n", status);
        LOG_INFO(classNAME, "Registration Status: %d", status);
        delay(1000);

        String ueInfo;
        if (modem.getSystemInformation(ueInfo)) {
            // Serial.print("Inquiring UE system information: ");
            // Serial.println(ueInfo);
            LOG_INFO(classNAME, "Inquiring UE system information: %s", ueInfo.c_str());
        }

        if (!modem.setNetworkActive()) {
            // Serial.println("Enable network failed!");
            LOG_ERROR(classNAME, "Enable network failed!");
            retval = false;
        }

        if (retval) {
            // Serial.println("Network active");
            LOG_INFO(classNAME, "Network active");
            
            delay(5000);
    
            String ipAddress = modem.getLocalIP();
            // Serial.print("Network IP: ");
            // Serial.println(ipAddress);
            // Serial.print("Network Signal Quality: ");
            // Serial.println(modem.getSignalQuality());
            LOG_INFO(classNAME, "Network IP: %s", ipAddress.c_str());
            LOG_INFO(classNAME, "Network Signal Quality: %d", modem.getSignalQuality());
    
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

            if (retval) {
                // Serial.println("Ping success!");
                LOG_INFO(classNAME, "Ping success!");
            } else {
                // Serial.println("Ping failed!");
                LOG_ERROR(classNAME, "Ping failed!");
            }
        } else {
            // Serial.println("Network not active");
            LOG_ERROR(classNAME, "Network not active");
        }
    } else {
        // Serial.println("Network not ready");
        LOG_ERROR(classNAME, "Network not ready");
    }

    return retval;
}

bool initWiFi()
{
    return initWiFi(NETWORK_SSID, NETWORK_PSK);
}

bool initWiFi(const char *ssid, const char *psk)
{
    bool retval = true;

    LOG_INFO(classNAME, "Start WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, psk);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
        if (retry++ > 10) {
            retval = false;
            break;
        }
    }

    if (retval) {
        LOG_INFO(classNAME, "WiFi connected successfully");

        IPAddress ip = WiFi.localIP();
        int32_t rssi = WiFi.RSSI();
        LOG_INFO(classNAME, "WiFi IP: %s", ip.toString().c_str());
        LOG_INFO(classNAME, "WiFi Signal Strength (RSSI): %d dBm", rssi);

        LOG_INFO(classNAME, "Pinging www.google.com...");
        if (Ping.ping("www.google.com")) {
            LOG_INFO(classNAME, "Ping success!");
        } else {
            LOG_ERROR(classNAME, "Ping failed!");
            retval = false;
        }
    } else {
        LOG_ERROR(classNAME, "WiFi connection failed");
    }

    return retval;
}