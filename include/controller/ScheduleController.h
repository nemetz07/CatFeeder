//
// Created by Norbi on 2020. 12. 18..
//

#ifndef CATFEEDER_SCHEDULECONTROLLER_H
#define CATFEEDER_SCHEDULECONTROLLER_H


#include <ESPAsyncWebServer.h>

class ScheduleController {
public:
    static void index(AsyncWebServerRequest *request);

    static void addAlarm(AsyncWebServerRequest *request);

private:
    static void processor();
};


#endif //CATFEEDER_SCHEDULECONTROLLER_H
