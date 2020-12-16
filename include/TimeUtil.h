//
// Created by Norbi on 2020. 12. 16..
//

#ifndef CATFEEDER_TIMEUTIL_H
#define CATFEEDER_TIMEUTIL_H


#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>

class TimeUtil {
public:
    static const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
    static const int timeZone = 1;
    static const unsigned int localPort = 8888;

    static void init();
    static void sendNTPpacket(IPAddress &address);
    static time_t getNtpTime();
    static void digitalClockDisplay();
    static void printDigits(int digits);
};


#endif //CATFEEDER_TIMEUTIL_H
