#include "network.h"

#define classNAME "network"

StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
TinyGPSPlus gps;

TinyGsmClient gsmClient(modem);
WiFiClient wifiClient;

HttpClient *httpClient = nullptr;
Thingsboard thingsboard;
MACs macs;
TimeStamp timeStamps;

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

                modem.sendAT("+CDNSGIP=\"www.google.com\"");
                modem.waitResponse(1000);

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

void setData(Thingsboard &tb, MACs &mac, TimeStamp &timeStamp) {
    if (tb.url.length() > 0) {
        thingsboard.url = tb.url;
        thingsboard.sn = tb.sn;
        thingsboard.keyProvisioning = tb.keyProvisioning;
        thingsboard.secretProvisioning = tb.secretProvisioning;
        thingsboard.token = tb.token;

        macs.url = mac.url;

        timeStamps.url = timeStamp.url;

        LOG_INFO(classNAME, "Thingsboard URL: %s", thingsboard.url.c_str());
        LOG_INFO(classNAME, "Thingsboard SN: %s", thingsboard.sn.c_str());
        LOG_INFO(classNAME, "Thingsboard Key Provisioning: %s", thingsboard.keyProvisioning.c_str());
        LOG_INFO(classNAME, "Thingsboard Secret Provisioning: %s", thingsboard.secretProvisioning.c_str());
        
        LOG_INFO(classNAME, "MACs URL: %s", macs.url.c_str());
    }
    else {
        LOG_ERROR(classNAME, "Thingsboard URL not configured");
    }
}

String sendHttpRequest(String url, String endpoint, String method, String payload) {
    String retval = "";

    if (httpClient == nullptr) {
        if (WiFi.isConnected()) {
            httpClient = new HttpClient(wifiClient, url);
            LOG_DEBUG(classNAME, "WiFi Client");
        }
        else {
            httpClient = new HttpClient(gsmClient, url);
            LOG_DEBUG(classNAME, "GSM Client");
        }
    }

    LOG_INFO(classNAME, "Sending request to http://%s%s", url.c_str(), endpoint.c_str());

    int errorCode = 0;

    httpClient->beginRequest();

    if (method == "POST") {
        // httpClient->post(endpoint.c_str(), "application/json", "");
        // httpClient->sendHeader("Content-Length", String(payload.length()));
        // httpClient->println(payload.c_str());

        String request =
            "POST " + endpoint + " HTTP/1.1\r\n" +
            "Host: " + url + "\r\n" +
            "User-Agent: Arduino/2.2.0\r\n" +
            "Content-Type: application/json\r\n" +
            "Content-Length: " + String(payload.length()) + "\r\n" +
            "Connection: close\r\n\r\n" +
            payload;

        httpClient->print(request);
    }
    else if (method == "GET") {
        // httpClient->get(endpoint.c_str());
        
        String request =
            "GET " + endpoint + " HTTP/1.1\r\n" +
            "Host: " + url + "\r\n" +
            "User-Agent: Arduino/2.2.0\r\n" +
            "Connection: close\r\n\r\n";
        httpClient->print(request);
    }
    else {
        LOG_ERROR(classNAME, "Unsupported HTTP method");

        httpClient->stop();
        delete httpClient;
        httpClient = nullptr;

        return "";
    }

    httpClient->endRequest();

    errorCode = httpClient->responseStatusCode();
    LOG_INFO(classNAME, "HTTP response code: %d", errorCode);
    
    if (errorCode == 0 || errorCode == 200 || errorCode == 201) {
        LOG_INFO(classNAME, "HTTP request sent successfully");

        retval = httpClient->responseBody();
        LOG_INFO(classNAME, "HTTP response body: %s", retval.c_str());
        LOG_INFO(classNAME, "Response length: %d", retval.length());
    }
    else {
        LOG_ERROR(classNAME, "Failed to send HTTP request");
        
        String response = httpClient->responseBody();
        LOG_ERROR(classNAME, "HTTP error: %d, %s", errorCode, response.length() > 0 ? response.c_str() : "No body");
    }

    httpClient->stop();
    delete httpClient;
    httpClient = nullptr;

    return retval;
}

void getToken() {
    LOG_INFO(classNAME, "Thingsboard SN: %s", thingsboard.sn.c_str());
    LOG_INFO(classNAME, "Thingsboard Key Provisioning: %s", thingsboard.keyProvisioning.c_str());
    LOG_INFO(classNAME, "Thingsboard Secret Provisioning: %s", thingsboard.secretProvisioning.c_str());

    if (thingsboard.secretProvisioning.length() > 0 && thingsboard.keyProvisioning.length() > 0) {
        LOG_INFO(classNAME, "Getting token from Thingsboard");

        String endpoint = "/api/v1/provision";
        LOG_INFO(classNAME, "ENDPOINT: %s", endpoint.c_str());
        
        String payload = "{\"deviceName\":\"" + thingsboard.sn + "\",\"provisionDeviceKey\":\"" + thingsboard.keyProvisioning + "\",\"provisionDeviceSecret\":\"" + thingsboard.secretProvisioning + "\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());
        
        String response = sendHttpRequest(thingsboard.url, endpoint, "POST", payload);

        if (response.length() > 0) {
            LOG_INFO(classNAME, "Parsing response");

            StaticJsonDocument<256> doc;
            DeserializationError error = deserializeJson(doc, response);

            if (error) {
                LOG_ERROR(classNAME, "JSON parsing failed: %s", error.c_str());
            }

            const char* status = doc["status"];
            const char* credentials = doc["credentialsValue"];

            if (String(status) == "SUCCESS" && credentials != nullptr) {
                String token;
                token = String(credentials);
                LOG_INFO(classNAME, "Token stored successfully, length: %d", token.length());
                LOG_INFO(classNAME, "Token: %s", token.c_str());

                thingsboard.token = token;

                updateConfigField("TOKEN", token);
            } else {
                LOG_ERROR(classNAME, "Provisioning failed. Status: %s", status);
            }
        } else {
            LOG_ERROR(classNAME, "Failed to get token from Thingsboard");
        }
    } else {
        LOG_ERROR(classNAME, "key and secret provisioning not configured for Thingsboard");
    }
}

bool postData(const String& key, const String& newValue) {
    LOG_DEBUG(classNAME, "Token: %s", thingsboard.token.c_str());

    if (thingsboard.token.c_str() != "null") {
        String endpoint = "/api/v1/" + thingsboard.token + "/attributes";
        LOG_INFO(classNAME, "ENDPOINT: %s", endpoint.c_str());
        
        String payload = "{\"" + key +"\":\"" + newValue + "\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());
        
        String response = sendHttpRequest(thingsboard.url, endpoint, "POST", payload);
    }
    else {
        LOG_ERROR(classNAME, "token not set");
    }

    return false;
}

bool postMac(const String& key, const String& newValue) {
    String endpoint = "/register";
    LOG_INFO(classNAME, "ENDPOINT: %s", endpoint.c_str());
    
    String payload = "{\"" + key +"\":\"" + newValue + "\"}";
    LOG_INFO(classNAME, "Payload: %s", payload.c_str());
    
    String response = sendHttpRequest(macs.url, endpoint, "POST", payload);

    if (response.length() == 0) {
        LOG_ERROR(classNAME, "Empty response from MAC server");
        return false;
    }

    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        LOG_ERROR(classNAME, "Failed to parse JSON response: %s", error.c_str());
        return false;
    }

    const char* status = doc["status"];
    if (status != nullptr && String(status) == "OK") {
        LOG_INFO(classNAME, "MAC registration successful");
        return true;
    } else {
        LOG_ERROR(classNAME, "MAC registration failed. Status: %s", status ? status : "null");
        return false;
    }
}

bool getUpdateAllData() {
    LOG_DEBUG(classNAME, "Token: %s", thingsboard.token.c_str());

    if (thingsboard.token.c_str() != "null") {
        String endpoint = "/api/v1/" + thingsboard.token + "/attributes";
        LOG_INFO(classNAME, "ENDPOINT: %s", endpoint.c_str());
        
        String response = sendHttpRequest(thingsboard.url, endpoint, "GET", "");

        if (response.length() == 0) {
            LOG_ERROR(classNAME, "Empty response from server");
            return false;
        }

        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            LOG_ERROR(classNAME, "Failed to parse JSON: %s", error.c_str());
            return false;
        }

        JsonObject shared = doc["shared"];
        if (shared.isNull()) {
            LOG_ERROR(classNAME, "No 'shared' object in JSON");
            return false;
        }

        Config cfg = readConfig();

        if (shared.containsKey("SN") && shared["SN"].as<String>() != cfg.sn)
            updateConfigField("SN", shared["SN"].as<String>());

        if (shared.containsKey("isWiFi"))
            updateConfigField("isWiFi", shared["isWiFi"].as<String>() == "true");

        if (shared.containsKey("isGPRS"))
            updateConfigField("isGPRS", shared["isGPRS"].as<String>() == "true");

        if (shared.containsKey("SSID") && shared["SSID"].as<String>() != cfg.ssid)
            updateConfigField("SSID", shared["SSID"].as<String>());

        if (shared.containsKey("PSWD") && shared["PSWD"].as<String>() != cfg.psk)
            updateConfigField("PSK", shared["PSWD"].as<String>());

        if (shared.containsKey("APN") && shared["APN"].as<String>() != cfg.apn)
            updateConfigField("APN", shared["APN"].as<String>());

        if (shared.containsKey("longitude") && shared["longitude"].as<float>() != cfg.longitude)
            updateConfigField("LONGITUDE", shared["longitude"].as<float>());

        if (shared.containsKey("latitude") && shared["latitude"].as<float>() != cfg.latitude)
            updateConfigField("LATITUDE", shared["latitude"].as<float>());

        if (shared.containsKey("flowMeter") && shared["flowMeter"].as<int>() != cfg.flowMeter)
            updateConfigField("flowMeter", shared["flowMeter"].as<int>());

        if (shared.containsKey("flowMeterTotal") && shared["flowMeterTotal"].as<int>() != cfg.flowMeterTotal)
            updateConfigField("flowMeterTotal", shared["flowMeterTotal"].as<int>());

        readAllConfig();
        return true;
    }
    else {
        LOG_ERROR(classNAME, "token not set");
    }

    return false;
}

bool getUpdateData(const String& key) {
    if (thingsboard.token == "null" || thingsboard.token.length() == 0) {
        LOG_ERROR(classNAME, "token not set");
        return false;
    }

    String endpoint = "/api/v1/" + thingsboard.token + "/attributes";
    LOG_INFO(classNAME, "URL: %s", endpoint.c_str());

    String response = sendHttpRequest(thingsboard.url, endpoint, "GET", "");
    LOG_DEBUG(classNAME, "response: %s", response.c_str());

    if (response.length() == 0) {
        LOG_ERROR(classNAME, "Empty response from server");
        return false;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        LOG_ERROR(classNAME, "Failed to parse JSON: %s", error.c_str());
        return false;
    }

    JsonObject shared = doc["shared"];
    if (shared.isNull()) {
        LOG_ERROR(classNAME, "No 'shared' object in JSON");
        return false;
    }

    if (!shared.containsKey(key)) {
        LOG_WARN(classNAME, "Key '%s' not found in response", key.c_str());
        return false;
    }

    Config cfg = readConfig();

    if (key == "SN") {
        String val = shared["SN"].as<String>();
        if (cfg.sn != val)
            updateConfigField("SN", val);
    }
    else if (key == "isWiFi") {
        bool val = shared["isWiFi"].as<String>() == "true";
        if (cfg.isWiFi != val)
            updateConfigField("isWiFi", val);
    }
    else if (key == "isGPRS") {
        bool val = shared["isGPRS"].as<String>() == "true";
        if (cfg.isGPRS != val)
            updateConfigField("isGPRS", val);
    }
    else if (key == "SSID") {
        String val = shared["SSID"].as<String>();
        if (cfg.ssid != val)
            updateConfigField("SSID", val);
    }
    else if (key == "PSWD") {
        String val = shared["PSWD"].as<String>();
        if (cfg.psk != val)
            updateConfigField("PSK", val);
    }
    else if (key == "APN") {
        String val = shared["APN"].as<String>();
        if (cfg.apn != val)
            updateConfigField("APN", val);
    }
    else if (key == "longitude") {
        float val = shared["longitude"].as<float>();
        if (cfg.longitude != val)
            updateConfigField("LONGITUDE", val);
    }
    else if (key == "latitude") {
        float val = shared["latitude"].as<float>();
        if (cfg.latitude != val)
            updateConfigField("LATITUDE", val);
    }
    else if (key == "flowMeter") {
        int val = shared["flowMeter"].as<int>();
        if (cfg.flowMeter != val)
            updateConfigField("flowMeter", val);
    }
    else if (key == "flowMeterTotal") {
        int val = shared["flowMeterTotal"].as<int>();
        if (cfg.flowMeterTotal != val)
            updateConfigField("flowMeterTotal", val);
    }
    else {
        LOG_WARN(classNAME, "Key '%s' not handled", key.c_str());
        return false;
    }
    
    return true;
}

void syncRTCESP32() {
    // configTime(-3 * 3600, 0, "pool.ntp.org");

    // LOG_INFO(classNAME, "Waiting for NTP synchronization...");
    // time_t now = time(nullptr);
    // unsigned long start = millis();
    // while (now < 8 * 3600 * 2 && millis() - start < 10000) {
    //     delay(500);
    //     now = time(nullptr);
    // }

    // if (now < 8 * 3600 * 2) {
    //     LOG_ERROR(classNAME, "Failed to get time from NTP.");
    //     return;
    // }

    // struct tm *timeinfo = localtime(&now);
    // if (timeinfo) {
    //     char buffer[30];
    //     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    //     LOG_DEBUG(classNAME, "System time (NTP): %s", buffer);
    // } else {
    //     LOG_ERROR(classNAME, "System time is invalid.");
    //     return;
    // }
    
    // time_t utcNow = now - (3 * 3600);
    
    // DateTime rtcTime = getRTC();
    // time_t rtcEpoch = rtcTime.unixtime();
    
    // LOG_DEBUG(classNAME, "Epoch NTP (UTC): %ld", utcNow);
    // LOG_DEBUG(classNAME, "Epoch RTC (UTC): %ld", rtcEpoch);

    // long diffSeconds = abs(utcNow - rtcEpoch);

    // LOG_DEBUG(classNAME, "RTC time: %04d-%02d-%02d %02d:%02d:%02d",
    //           rtcTime.year(), rtcTime.month(), rtcTime.day(),
    //           rtcTime.hour(), rtcTime.minute(), rtcTime.second());

    // LOG_INFO(classNAME, "RTC vs NTP time difference: %ld seconds", diffSeconds);

    // if (diffSeconds >= 3600) {
    //     LOG_INFO(classNAME, "Time difference is too large, updating RTC...");
    //     setRTCDateTime(now);
    // } else {
    //     LOG_INFO(classNAME, "RTC is already synchronized. No update needed.");
    // }

    String json = sendHttpRequest(timeStamps.url.c_str(), "", "GET", "");
    LOG_INFO(classNAME, "Response NTP: %s", json.c_str());

    // StaticJsonDocument<1024> doc;
    // DeserializationError error = deserializeJson(doc, json);
    // if (error) {
    //     LOG_ERROR(classNAME, "Failed to parse NTP JSON: %s", error.c_str());
    //     return;
    // }

    // if (!doc.containsKey("unixtime")) {
    //     LOG_ERROR(classNAME, "NTP JSON missing 'unixtime' field");
    //     return;
    // }

    // time_t ntpEpoch = doc["unixtime"];
    // struct timeval now = { .tv_sec = ntpEpoch, .tv_usec = 0 };
    
    // settimeofday(&now, nullptr);
    // LOG_INFO(classNAME, "System time updated from NTP (epoch: %ld)", ntpEpoch);

    // time_t sysTime = time(nullptr);
    // struct tm* timeinfo = localtime(&sysTime);
    // if (timeinfo) {
    //     char buffer[30];
    //     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    //     LOG_DEBUG(classNAME, "System time (local): %s", buffer);
    // }

    // DateTime rtcTime = getRTC();
    // time_t rtcEpoch = rtcTime.unixtime();

    // LOG_DEBUG(classNAME, "Epoch RTC (UTC): %ld", rtcEpoch);

    // long diffSeconds = abs((ntpEpoch - 3 * 3600) - rtcEpoch);
    // LOG_DEBUG(classNAME, "RTC time: %04d-%02d-%02d %02d:%02d:%02d",
    //           rtcTime.year(), rtcTime.month(), rtcTime.day(),
    //           rtcTime.hour(), rtcTime.minute(), rtcTime.second());

    // LOG_INFO(classNAME, "RTC vs NTP time difference: %ld seconds", diffSeconds);

    // if (diffSeconds >= 3600) {
    //     LOG_INFO(classNAME, "Time difference is too large, updating RTC...");
    //     setRTCDateTime(ntpEpoch);
    // } else {
    //     LOG_INFO(classNAME, "RTC is already synchronized. No update needed.");
    // }
}

void sendMAC() {
    String mac = WiFi.macAddress();
    LOG_INFO(classNAME, "WiFi MAC Address: %s", mac.c_str());

    if (!postMac("mac", mac)) {
        LOG_ERROR(classNAME, "Failed to send MAC address");
    } else {
        LOG_INFO(classNAME, "MAC address sent successfully");
    }
}