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
        // Serial.println("Thingsboard URL and Device ID configured");
        LOG_INFO(classNAME, "Thingsboard URL and Device ID configured");

        thingsboard.url = tb.url;
        thingsboard.port = tb.port;
        thingsboard.deviceID = tb.deviceID;
        thingsboard.username = tb.username;
        thingsboard.password = tb.password;
    } else {
        // Serial.println("Thingsboard URL and Device ID not configured");
        LOG_ERROR(classNAME, "Thingsboard URL and Device ID not configured");
    }
}

String sendHttpRequest(String url, String method, String payload) {
    if (httpClient == nullptr) {
        if (WiFi.isConnected()) {
            httpClient = new HttpClient(wifiClient, thingsboard.url.c_str(), thingsboard.port.toInt());
        } else if (modem.isNetworkConnected()) {
            httpClient = new HttpClient(gsmClient, thingsboard.url.c_str(), thingsboard.port.toInt());
        }
    }

    int errorCode = 0;

    if (method == "GET") {
        httpClient->sendHeader("Content-Type", "application/json");
        httpClient->sendHeader("Accept", "application/json");

        if (token.length() > 0) {
            httpClient->sendHeader("X-Authorization", "Bearer " + token);
        }

        httpClient->beginRequest();
        httpClient->get(url.c_str());
    }
    else if (method == "POST") {
        httpClient->sendHeader("Content-Type", "application/json");
        httpClient->sendHeader("Accept", "application/json");
        httpClient->sendHeader("X-Authorization", "Bearer " + token);
        httpClient->beginRequest();
        httpClient->post(url.c_str(), "", payload.c_str());
    }
    else {
        // Serial.println("Unsupported HTTP method");
        LOG_ERROR(classNAME, "Unsupported HTTP method");
        return "";
    }
    
    if (errorCode == 0) {
        // Serial.println("HTTP request sent successfully");
        LOG_INFO(classNAME, "HTTP request sent successfully");

        int statusCode = httpClient->responseStatusCode();
        LOG_INFO(classNAME, "HTTP status code: %d", statusCode);
        if (statusCode == 200 || statusCode == 201) {
            // Serial.println("HTTP response received successfully");
            LOG_INFO(classNAME, "HTTP response received successfully");

            String responseBody = httpClient->responseBody();
            LOG_INFO(classNAME, "HTTP response body: %s", responseBody.c_str());
            LOG_INFO(classNAME, "Response length: %d", responseBody.length());

            return responseBody;
        } else {
            // Serial.println("HTTP error: " + String(statusCode));
            LOG_ERROR(classNAME, "HTTP error: %d, %s", statusCode, httpClient->responseBody().c_str());
        }
    } else {
        // Serial.println("Failed to send HTTP request");
        LOG_ERROR(classNAME, "Failed to send HTTP request");
    }

    return "";
}

bool getToken() {
    bool retval = false;

    if (thingsboard.username.length() > 0 && thingsboard.password.length() > 0) {
        // Serial.println("Getting token from Thingsboard");
        LOG_INFO(classNAME, "Getting token from Thingsboard");

        LOG_INFO(classNAME, "WiFi status: %d", WiFi.status());
        LOG_INFO(classNAME, "WiFi IP: %s", WiFi.localIP().toString().c_str());

        // String url = "https://" + String(thingsboard.url) + "/api/auth/login";
        String url = "/api/auth/login";
        LOG_INFO(classNAME, "URL: %s", url.c_str());
        String payload = "{\"username\":\"" + thingsboard.username + "\",\"password\":\"" + thingsboard.password + "\"}";
        LOG_INFO(classNAME, "Payload: %s", payload.c_str());
        String response = sendHttpRequest(url, "POST", payload);

        if (response.length() > 0) {
            // Serial.println("Token received successfully");
            LOG_INFO(classNAME, "Token received successfully");

            // Parse the token from the response
            int startIndex = response.indexOf("\"token\":\"") + 9;
            int endIndex = response.indexOf("\"", startIndex);
            token = response.substring(startIndex, endIndex);

            if (token.length() > 0) {
                LOG_INFO(classNAME, "Token stored successfully, length: %d", token.length());
                LOG_INFO(classNAME, "Token (parte 1): %.100s...", token.c_str());

                if (token.length() > 100) {
                    for (int i = 100; i < token.length(); i += 100) {
                        String chunk = token.substring(i, i + 100);
                        LOG_INFO(classNAME, "Token (parte siguiente): %.100s...", chunk.c_str());
                    }
                }

                retval = true;
            }
        } else {
            // Serial.println("Failed to get token from Thingsboard");
            LOG_ERROR(classNAME, "Failed to get token from Thingsboard");
        }
    } else {
        // Serial.println("Username and password not configured for Thingsboard");
        LOG_ERROR(classNAME, "Username and password not configured for Thingsboard");
    }

    return retval;
}

void getDeviceData() {
    if (token.length() > 0) {
        // Serial.println("Getting device data from Thingsboard");
        LOG_INFO(classNAME, "Getting device data from Thingsboard");

        // String url = "https://" + String(thingsboard.url) + "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/values/attributes/SHARED_SCOPE";
        String url = "/api/plugins/telemetry/DEVICE/" + thingsboard.deviceID + "/values/attributes/SHARED_SCOPE";
        LOG_INFO(classNAME, "URL: %s", url.c_str());
        String response = sendHttpRequest(url, "GET", "");

        if (response.length() > 0) {
            // Serial.println("Device data received successfully");
            LOG_INFO(classNAME, "Device data received successfully");

            // Process the response as needed
            // For example, parse the JSON response and extract the values
            // Serial.println("Response: " + response);
            LOG_INFO(classNAME, "Response: %s", response.c_str());
        } else {
            // Serial.println("Failed to get device data from Thingsboard");
            LOG_ERROR(classNAME, "Failed to get device data from Thingsboard");
        }
    } else {
        // Serial.println("Token not available for Thingsboard");
        LOG_ERROR(classNAME, "Token not available for Thingsboard");
    }
}

void postDeviceData() {

}