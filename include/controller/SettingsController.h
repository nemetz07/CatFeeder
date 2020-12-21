//
// Created by Norbi on 2020. 12. 17..
//

#ifndef CATFEEDER_SETTINGSCONTROLLER_H
#define CATFEEDER_SETTINGSCONTROLLER_H

#define SETTINGS_HTML "/settings.html"

#include <ESPAsyncWebServer.h>

class SettingsController {
public:
    static void index(AsyncWebServerRequest *request);

    static void setServoPin(AsyncWebServerRequest *request);

    static void testMotor(AsyncWebServerRequest *request);

    static void giveFoodManual(AsyncWebServerRequest *request);

    static void resetCooldown(AsyncWebServerRequest *request);
private:
    static String processor(const String &var);
};


#endif //CATFEEDER_SETTINGSCONTROLLER_H
