//
// Created by Norbi on 2020. 12. 16..
//

#ifndef CATFEEDER_TIMEUTIL_H
#define CATFEEDER_TIMEUTIL_H


#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#define TIMEZONE 1
#define NTP_PACKET_SIZE 48
#define LOCAL_PORT 8888
#define REFRESH_RATE 3600

class TimeUtil {
public:
    static void init();

    static void sendNTPpacket(IPAddress &address);

    static time_t getNtpTime();

    static void digitalClockDisplay();

    static void printDigits(int digits);
};


#endif //CATFEEDER_TIMEUTIL_H
