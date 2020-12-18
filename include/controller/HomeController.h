//
// Created by Norbi on 2020. 12. 17..
//

#ifndef CATFEEDER_HOMECONTROLLER_H
#define CATFEEDER_HOMECONTROLLER_H


#include <ESPAsyncWebServer.h>

class HomeController {
public:
    static void index(AsyncWebServerRequest *request);

    static String processor(const String &var);
};


#endif //CATFEEDER_HOMECONTROLLER_H
