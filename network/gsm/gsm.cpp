#include "gsm.h"



// bool testAT()
// {
//     return modem.testAT(1000);
// }

// bool checkSIMStatus()
// {
//     SimStatus sim = modem.getSimStatus();

//     return sim == SIM_READY;
// }

// String getModemInfo()
// {
//     return modem.getModemInfo();
// }

// int setNetworkAPN(String APN)
// {
//     modem.sendAT(GF("+CGDCONT=1,\"IP\",\""), APN, "\"");
    
//     return modem.waitResponse();
// }

// RegStatus getRegistrationStatus()
// {
//     return modem.getRegistrationStatus();
// }

// int getSignalQuality()
// {
//     return modem.getSignalQuality();
// }

// bool setNetworkActive()
// {
//     return modem.setNetworkActive();
// }

// String getLocalIP()
// {
//     return modem.getLocalIP();
// }

// bool ping(String host, String &resolved_ip_addr, uint32_t &rep_data_packet_size, uint32_t &tripTime, uint8_t &TTL)
// {
//     return modem.ping(host.c_str(), resolved_ip_addr, rep_data_packet_size, tripTime, TTL);
// }