//
// Created by Norbi on 2020. 12. 18..
//

#ifndef CATFEEDER_SCHEDULECONTROLLER_H
#define CATFEEDER_SCHEDULECONTROLLER_H


#include <ESPAsyncWebServer.h>

class ScheduleController {
public:
    static void index(AsyncWebServerRequest *request);

    static void setSchedule(AsyncWebServerRequest *request);

    static void deleteSchedule(AsyncWebServerRequest *request);
private:
    static String processor(const String &var);

};


#endif //CATFEEDER_SCHEDULECONTROLLER_H
