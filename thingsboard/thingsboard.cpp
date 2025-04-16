#include "thingsboard.h"

#define classNAME "thingsboard"

extern TinyGsm modem;
TinyGsmClient gsmClient(modem);
WiFiClient wifiClient;

HttpClient *httpClient = nullptr;
Thingsboard thingsboard;

String token;

void setData(Thingsboard &tb) {
    if (tb.url.length() > 0 && tb.deviceID.length() > 0) {
        LOG_INFO(classNAME, "Thingsboard URL and Device ID configured");

        thingsboard.url = tb.url;
        thingsboard.port = tb.port;
        thingsboard.deviceID = tb.deviceID;
        thingsboard.username = tb.username;
        thingsboard.password = tb.password;
    } else {
        LOG_ERROR(classNAME, "Thingsboard URL and Device ID not configured");
    }
}

String sendHttpRequest(String url, String method, String payload) {
    String retval = "";

    if (httpClient == nullptr) {
        if (WiFi.isConnected()) {
            httpClient = new HttpClient(wifiClient, thingsboard.url.c_str());
        } else if (modem.isNetworkConnected()) {
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
        } else {
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
    
    if (errorCode == 0) {
        LOG_INFO(classNAME, "HTTP request sent successfully");

        int statusCode = httpClient->responseStatusCode();
        LOG_INFO(classNAME, "HTTP status code: %d", statusCode);
        
        if (statusCode == 200 || statusCode == 201) {
            LOG_INFO(classNAME, "HTTP response received successfully");

            retval = httpClient->responseBody();
            LOG_INFO(classNAME, "HTTP response body: %s", retval.c_str());
            LOG_INFO(classNAME, "Response length: %d", retval.length());
        }
        else {
            String response = httpClient->responseBody();
            LOG_ERROR(classNAME, "HTTP error: %d, %s", statusCode, response.length() > 0 ? response.c_str() : "No body");
        }
    } else {
        LOG_ERROR(classNAME, "Failed to send HTTP request");
    }

    httpClient->stop();
    httpClient = nullptr;

    return retval;
}

bool getToken() {
    bool retval = false;

    if (thingsboard.username.length() > 0 && thingsboard.password.length() > 0) {
        LOG_INFO(classNAME, "Getting token from Thingsboard");

        LOG_INFO(classNAME, "WiFi status: %d", WiFi.status());
        LOG_INFO(classNAME, "WiFi IP: %s", WiFi.localIP().toString().c_str());

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
        } else {
            LOG_ERROR(classNAME, "Failed to get token from Thingsboard");
        }
    } else {
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
        } else {
            LOG_ERROR(classNAME, "Failed to get device data from Thingsboard");
        }
    } else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData() {
    if (token.length() > 0) {
        LOG_INFO(classNAME, "Posting device data to Thingsboard");

        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());

        String payload = "{\"data-prueba\":\"2\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());

        String response = sendHttpRequest(url, "POST", payload);

        LOG_INFO(classNAME, "Device data posted successfully");
    } else {
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}