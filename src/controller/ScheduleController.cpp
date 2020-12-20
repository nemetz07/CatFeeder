//
// Created by Norbi on 2020. 12. 18..
//

#include <FoodDispenser.h>
#include <SPIFFS.h>
#include <Constants.h>
#include "controller/ScheduleController.h"

void ScheduleController::setSchedule(AsyncWebServerRequest *request) {
    Serial.println("ScheduleController - set schedule");

    if (!request->hasArg("hour")) {
        Serial.println("No hour param!");
        request->send(400, "text/plain", "Error: param 'hour' is missing");
        return;
    }

    int hour = (request->arg("hour")).toInt();
    int minute = (request->arg("minute")).toInt();
    int portion = (request->arg("portion")).toInt();
    bool isDaily = (request->arg("isDaily")).equals("true");

    Serial.println("Params: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(" | ");
    Serial.print(portion);
    Serial.print(" | ");
    Serial.println(isDaily);

    FoodDispenser::getInstance().addAlarm(hour, minute, portion, isDaily);
    request->send(200, "text/plain", "success");
}

void ScheduleController::deleteSchedule(AsyncWebServerRequest *request) {
    Serial.println("ScheduleController - delete schedule");
    FoodDispenser::getInstance().deleteAlarm(true);

    request->send(200, "text/plain", "success");
}

void ScheduleController::index(AsyncWebServerRequest *request) {
    Serial.println("Schedule - index");
    request->send(SPIFFS, "/schedule.html", String(), false, processor);
}

String ScheduleController::processor(const String &var) {
    if (var == "TITLE") {
        return TITLE;
    } else if (var == "YEAR") {
        return YEAR;
    } else if (var == "PAGE_TITLE") {
        return "Etetés időzítése";
    } else if (var == "TIME") {
        return FoodDispenser::getInstance().getNextDispenseTime();
    } else if (var == "IS_DAILY_COLOR") {
        return FoodDispenser::getInstance().isAlarmDaily() ? ICON_TRUE_COLOR : ICON_FALSE_COLOR;
    } else if (var == "IS_DAILY_ICON") {
        return FoodDispenser::getInstance().isAlarmDaily() ? ICON_TRUE : ICON_FALSE;
    }

    return "-";
}
