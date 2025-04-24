#include "network.h"

#define classNAME "network"

StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
TinyGPSPlus gps;

bool initGSM()
{
    return initGSM(NETWORK_APN);
}

bool initGSM(const char *apn)
{
    bool retval = true;

    LOG_INFO(classNAME, "Start modem...");
    int retry = 0;

    while (!modem.testAT(1000)) {
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
    
    LOG_INFO(classNAME, "Waiting for network...");
    
    SimStatus status = modem.getSimStatus();
    int simRetries = 0;
    while (status != SIM_READY && simRetries < 1) {
        LOG_INFO(classNAME, "SIM status: %d", status);
        LOG_INFO(classNAME, "SIM retries: %d", simRetries);
        simRetries++;

        switch (status) {
            case SIM_LOCKED:
                LOG_ERROR(classNAME, "SIM locked");
                retval = false;
                break;
            case SIM_READY:
                LOG_INFO(classNAME, "SIM ready");
                break;
            case SIM_ERROR:
            default:
                    LOG_ERROR(classNAME, "SIM not inserted or unknown SIM status: %d", status);
                    retval = false;
                    break;
        }

        if (status) {
            LOG_ERROR(classNAME, "SIM not ready, please check the SIM card!");
            break;
        }

        status = modem.getSimStatus();

        delay(1000);
    }

    if (retval) {
        LOG_INFO(classNAME, "Network ready");

        modem.sendAT("+SIMCOMATI");
        modem.waitResponse();

        Serial.printf("Set network apn : %s\n", apn);
        modem.sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, "\"");
        if (modem.waitResponse() != 1) {

            LOG_ERROR(classNAME, "Set network apn error!");
            retval = false;
        }

        if (!retval) {

            LOG_ERROR(classNAME, "Network not ready");
            return false;
        }

        int16_t sq ;
        LOG_INFO(classNAME, "Wait for the modem to register with the network.");
        RegStatus status = REG_NO_RESULT;
        while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED) {
            Serial.print(".");
            status = modem.getRegistrationStatus();

            switch (status) {
                case REG_UNREGISTERED:
                case REG_SEARCHING:
                    sq = modem.getSignalQuality();
        
                    LOG_INFO(classNAME, "Signal Quality: %d", sq);
                    delay(1000);
                    break;
                case REG_DENIED:
        
                    LOG_ERROR(classNAME, "Network registration was rejected, please check if the APN is correct");
                    retval = false;
                    break;
                case REG_OK_HOME:
        
                    LOG_INFO(classNAME, "Online registration successful");
                    break;
                case REG_OK_ROAMING:
        
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

            LOG_ERROR(classNAME, "Network not ready");
            return false;
        }

        LOG_INFO(classNAME, "Registration Status: %d", status);
        delay(1000);

        String ueInfo;
        if (modem.getSystemInformation(ueInfo)) {


            LOG_INFO(classNAME, "Inquiring UE system information: %s", ueInfo.c_str());
        }

        if (!modem.setNetworkActive()) {

            LOG_ERROR(classNAME, "Enable network failed!");
            retval = false;
        }

        if (retval) {

            LOG_INFO(classNAME, "Network active");
            
            delay(5000);
    
            String ipAddress = modem.getLocalIP();

            LOG_INFO(classNAME, "Network IP: %s", ipAddress.c_str());
            LOG_INFO(classNAME, "Network Signal Quality: %d", modem.getSignalQuality());
    
            String resolved_ip_addr;
            uint32_t rep_data_packet_size;
            uint32_t tripTime;
            uint8_t TTL;
            for (int i = 0; i < 10; ++i) {
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
    
                LOG_INFO(classNAME, "Ping success!");
            } else {
    
                LOG_ERROR(classNAME, "Ping failed!");
            }
        } else {

            LOG_ERROR(classNAME, "Network not active");
        }
    } else {
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
    WiFi.setAutoReconnect(false);
    WiFi.disconnect(true);
    delay(100);
    WiFi.begin(ssid, psk);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 10) {
        Serial.print(".");
        delay(1000);

        LOG_INFO(classNAME, "WiFi status: %d", WiFi.status());
        LOG_INFO(classNAME, "WiFi retry: %d", retry);

        retry++;

        if (retry == 10) {
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

    IPAddress ip;
    if (WiFi.hostByName("thingsboard.iotmrpicomp.com", ip)) {
        LOG_INFO(classNAME, "Resolved IP: %s", ip.toString().c_str());
    } else {
        LOG_ERROR(classNAME, "DNS resolution failed");
    }

    return retval;
}

bool initGPS()
{
    LOG_INFO(classNAME, "Starting GPS initialization...");

    String modemName = modem.getModemName();
    if (modemName == "UNKOWN") {
        LOG_ERROR(classNAME, "Unable to get modem name.");

        return false;
    }

    if (modemName.startsWith("A7670G")) {
        LOG_ERROR(classNAME, "A7670G does not support built-in GPS. Please use GPSShield example.");

        return false;
    }

    LOG_INFO(classNAME, "Modem model: %s", modemName.c_str());

    LOG_INFO(classNAME, "Enabling GPS...");
    if (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
        LOG_ERROR(classNAME, "Failed to enable GPS.");

        return false;
    }

    modem.setGPSBaud(MODEM_BAUDRATE);

    modem.setGPSMode(MODEM_GPS_MODE);

    modem.configNMEASentence(MODEM_GPS_CGA, MODEM_GPS_GLL, MODEM_GPS_GSA, MODEM_GPS_GSV, MODEM_GPS_RMC, MODEM_GPS_VTG, MODEM_GPS_ZDA, MODEM_GPS_ANT);

    modem.setGPSOutputRate(MODEM_GPS_OUTPUT_RATE);

    modem.enableNMEA();

    LOG_INFO(classNAME, "GPS initialization completed.");
    
    return true;
}

bool getGPSLocation(float &latitude, float &longitude)
{
    LOG_INFO(classNAME, "Checking GPS status...");

    if (!modem.isEnableGPS()) {
        LOG_ERROR(classNAME, "GPS is not enabled.");

        if (initGPS()) {
            LOG_INFO(classNAME, "GPS enabled successfully");
        } else {
            LOG_ERROR(classNAME, "Failed to enable GPS.");

            return false;
        }
    }
    else {
        LOG_INFO(classNAME, "GPS enabled successfully!");
    }

    LOG_INFO(classNAME, "Waiting for GPS fix...");

    unsigned long start = millis();
    const unsigned long timeout = 60000;

    while (millis() - start < timeout) {
        while (SerialAT.available()) {
            gps.encode(SerialAT.read());
        }

        if (gps.location.isUpdated() && gps.location.isValid()) {
            latitude = gps.location.lat();
            longitude = gps.location.lng();

            LOG_INFO(classNAME, "Latitude: %.6f, Longitude: %.6f", latitude, longitude);

            return true;
        }

        delay(100);
    }

    LOG_ERROR(classNAME, "Timeout waiting for valid GPS fix");

    return false;
}