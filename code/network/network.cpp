#include "network.h"

#define classNAME "network"

StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
TinyGPSPlus gps;

TinyGsmClient gsmClient(modem);
WiFiClient wifiClient;

HttpClient *httpClient = nullptr;
Thingsboard thingsboard;

String token;

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

    if (!modem.init()) {
        LOG_ERROR(classNAME, "Failed to start modem");
        return false;
    }
    
    LOG_INFO(classNAME, "Waiting for network...");
    
    SimStatus status = modem.getSimStatus();
    int simRetries = 0;
    while (status != SIM_READY && simRetries < 10) {
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

            modem.sendAT("+CSQ");
            modem.waitResponse(1000, "+CSQ: ");

            modem.sendAT("+COPS?");
            modem.waitResponse(1000, "+COPS: ");

            modem.sendAT("+CGDCONT?");
            modem.waitResponse(1000, "+CGDCONT: ");

            modem.sendAT("+NETOPEN?");
            modem.waitResponse(1000, "+NETOPEN: ");

            modem.sendAT("+IPADDR");
            modem.waitResponse(1000, "+IPADDR: ");

            modem.sendAT("+CDNSCFG?");
            modem.waitResponse(1000, "+CDNSCFG: ");
            
            delay(5000);
    
            String ipAddress = modem.getLocalIP();

            LOG_INFO(classNAME, "Network IP: %s", ipAddress.c_str());
            LOG_INFO(classNAME, "Network Signal Quality: %d", modem.getSignalQuality());

            // if (!modem.gprsConnect(apn)) {
            //     LOG_ERROR(classNAME, "GPRS connection failed!");
            //     retval = false;
            // } else {
            //     LOG_INFO(classNAME, "GPRS connected successfully");
            // }

            if (retval)
            {
                if (modem.isGprsConnected()) {
                    LOG_INFO(classNAME, "GPRS is connected");
                } else {
                    LOG_ERROR(classNAME, "GPRS is not connected");
                    retval = false;
                }
        
                String resolved_ip_addr;
                uint32_t rep_data_packet_size;
                uint32_t tripTime;
                uint8_t TTL;
                for (int i = 0; i < 10; ++i) {
                    int res = modem.ping("8.8.8.8", resolved_ip_addr, rep_data_packet_size, tripTime, TTL);
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
            }
            else {
                LOG_ERROR(classNAME, "GPRS connection failed!");
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

bool getCoordinatesInternet(float &latitude, float &longitude)
{
    bool retval = false;

    LOG_INFO(classNAME, "Getting coordinates from internet...");

    if (httpClient == nullptr) {
        if (WiFi.isConnected()) {
            httpClient = new HttpClient(wifiClient, "ipinfo.io");
        }
        else {
            httpClient = new HttpClient(gsmClient, "ipinfo.io");
        }
    }
    
    int errorCode = 0;
    String url = "/json";

    httpClient->beginRequest();
    httpClient->get(url.c_str());
    httpClient->sendHeader("Content-Type", "application/json");

    httpClient->endRequest();

    errorCode = httpClient->responseStatusCode();
    LOG_INFO(classNAME, "HTTP response code: %d", errorCode);

    if (errorCode == 200) {
        String response = httpClient->responseBody();
        LOG_INFO(classNAME, "HTTP response body: %s", response.c_str());

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            LOG_ERROR(classNAME, "Failed to parse JSON: %s", error.c_str());

            httpClient->stop();
            httpClient = nullptr;

            return false;
        }

        String loc = doc["loc"].as<String>();
        int commaIndex = loc.indexOf(',');

        if (commaIndex != -1) {
            latitude = loc.substring(0, commaIndex).toFloat();
            longitude = loc.substring(commaIndex + 1).toFloat();
        }

        LOG_INFO(classNAME, "Latitude: %.6f, Longitude: %.6f", latitude, longitude);

        retval = true;
    }
    else {
        LOG_ERROR(classNAME, "Failed to get coordinates from internet");
        LOG_ERROR(classNAME, "HTTP error: %d", errorCode);

        retval = false;
    }

    httpClient->stop();
    httpClient = nullptr;

    return retval;
}

void setData(Thingsboard &tb) {
    if (tb.url.length() > 0 && tb.deviceID.length() > 0) {
        LOG_INFO(classNAME, "Thingsboard URL and Device ID configured");

        thingsboard.url = tb.url;
        thingsboard.port = tb.port;
        thingsboard.deviceID = tb.deviceID;
        thingsboard.username = tb.username;
        thingsboard.password = tb.password;
    }
    else {
        LOG_ERROR(classNAME, "Thingsboard URL and Device ID not configured");
    }
}

String sendHttpRequest(String url, String method, String payload) {
    String retval = "";

    if (httpClient == nullptr) {
        if (WiFi.isConnected()) {
            httpClient = new HttpClient(wifiClient, thingsboard.url.c_str());
        }
        else {
            httpClient = new HttpClient(gsmClient, thingsboard.url.c_str());
        }
    }

    int errorCode = 0;

    if (method == "GET") {
        httpClient->beginRequest();
        httpClient->get(url.c_str());
        
        httpClient->sendHeader("Content-Type", "application/json");
        httpClient->sendHeader("Accept", "application/json");

        LOG_INFO(classNAME, "Token length: %d", token.length());

        if (token.length() > 0) {
            httpClient->sendHeader("X-Authorization", "Bearer " + token);
        }

        httpClient->endRequest();
    }
    else if (method == "POST") {
        httpClient->beginRequest();
        
        if (payload.indexOf("\"username\"") != -1) {
            LOG_INFO(classNAME, "Payload contains username, using POST method");

            httpClient->post(url.c_str(), "", payload.c_str());
            httpClient->sendHeader("Content-Type", "application/json");
            httpClient->sendHeader("Accept", "application/json");
        }
        else {
            httpClient->post(url.c_str(), "application/json", "");
            
            LOG_INFO(classNAME, "Token length: %d", token.length());
            
            if (token.length() > 0) {
                httpClient->sendHeader("X-Authorization", "Bearer " + token);
            }

            httpClient->sendHeader("Content-Length", String(payload.length()));
            httpClient->println(payload.c_str());
        }

        httpClient->endRequest();
    }
    else {
        LOG_ERROR(classNAME, "Unsupported HTTP method");
        
        return "";
    }

    errorCode = httpClient->responseStatusCode();
    LOG_INFO(classNAME, "HTTP response code: %d", errorCode);
    
    if (errorCode == 0 || errorCode == 200 || errorCode == 201) {
        LOG_INFO(classNAME, "HTTP request sent successfully");

        retval = httpClient->responseBody();
        LOG_INFO(classNAME, "HTTP response body: %s", retval.c_str());
        LOG_INFO(classNAME, "Response length: %d", retval.length());

        int statusCode = httpClient->responseStatusCode();
        LOG_INFO(classNAME, "HTTP status code: %d", statusCode);
        
        if (statusCode == 200 || statusCode == 201) {
        }
        else {
        }
    }
    else {
        LOG_ERROR(classNAME, "Failed to send HTTP request");
        
        String response = httpClient->responseBody();
        LOG_ERROR(classNAME, "HTTP error: %d, %s", errorCode, response.length() > 0 ? response.c_str() : "No body");
    }

    httpClient->stop();
    httpClient = nullptr;

    return retval;
}

bool getToken() {
    bool retval = false;

    if (thingsboard.username.length() > 0 && thingsboard.password.length() > 0) {
        LOG_INFO(classNAME, "Getting token from Thingsboard");

        String url = "/api/auth/login";
        LOG_INFO(classNAME, "URL: %s", url.c_str());
        
        String payload = "{\"username\":\"" + thingsboard.username + "\",\"password\":\"" + thingsboard.password + "\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());
        
        String response = sendHttpRequest(url, "POST", payload);

        if (response.length() > 0) {
            LOG_INFO(classNAME, "Token received successfully");

            int startIndex = response.indexOf("\"token\":\"") + 9;
            int endIndex = response.indexOf("\"", startIndex);
            token = response.substring(startIndex, endIndex);

            if (token.length() > 0) {
                LOG_INFO(classNAME, "Token stored successfully, length: %d", token.length());
                LOG_INFO(classNAME, "Token: %s", token.c_str());

                retval = true;
            }
        }
        else {
            LOG_ERROR(classNAME, "Failed to get token from Thingsboard");
        }
    }
    else {
        LOG_ERROR(classNAME, "Username and password not configured for Thingsboard");
    }

    return retval;
}

void getDeviceData() {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Getting device data from Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/values/attributes/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());
        String response = sendHttpRequest(url, "GET", "");

        if (response.length() > 0) {
            LOG_INFO(classNAME, "Device data received successfully");
            LOG_INFO(classNAME, "Response: %s", response.c_str());
        }
        else {
            LOG_ERROR(classNAME, "Failed to get device data from Thingsboard");
        }
    }
    else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData(String key, int value) {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Posting device data to Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());

        String payload = "{\""+ key + "\":" + String(value) + "}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());

        String response = sendHttpRequest(url, "POST", payload);

        LOG_INFO(classNAME, "Device data posted successfully");
    }
    else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData(String key, float value) {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Posting device data to Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());

        String payload = "{\""+ key + "\":" + String(value) + "}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());

        String response = sendHttpRequest(url, "POST", payload);

        LOG_INFO(classNAME, "Device data posted successfully");
    }
    else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData(String key, String value) {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Posting device data to Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());

        String payload = "{\""+ key + "\":\"" + value + "\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());

        String response = sendHttpRequest(url, "POST", payload);

        LOG_INFO(classNAME, "Device data posted successfully");
    }
    else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData(String key, bool value) {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Posting device data to Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());

        String payload = "{\""+ key + "\":" + String(value ? "true" : "false") + "}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());

        String response = sendHttpRequest(url, "POST", payload);

        LOG_INFO(classNAME, "Device data posted successfully");
    }
    else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}