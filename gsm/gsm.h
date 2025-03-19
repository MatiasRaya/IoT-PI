#include "includes/utilities.h"

bool checkSimCard();
bool connetToNetwork(const char* apn);
String sendATCommand(const char* command, uint32_t timeout);
int getSignalQuality();
String getLocalIP();
bool sendHttpGetRequest();
bool sendHttpPostRequest();