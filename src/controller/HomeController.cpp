//
// Created by Norbi on 2020. 12. 17..
//

#include <SPIFFS.h>
#include <FoodDispenser.h>
#include "controller/HomeController.h"
#include "Constants.h"

void HomeController::index(AsyncWebServerRequest *request) {
    Serial.println("Home - index");
    request->send(SPIFFS, "/index.html", String(), false, processor);
}

String HomeController::processor(const String &var) {
    if (var == "TITLE") {
        return TITLE;
    } else if (var == "YEAR") {
        return YEAR;
    } else if (var == "PAGE_TITLE") {
        return "Főoldal";
    } else if (var == "LAST_DISPENSE_TIME") {
        return FoodDispenser::getInstance().getLastDispenseTime();
    } else if (var == "LAST_DISPENSE_DATE") {
        return FoodDispenser::getInstance().getLastDispenseDate();
    } else if (var == "NEXT_DISPENSE_TIME") {
        return FoodDispenser::getInstance().getNextDispenseTime();
    } else if (var == "NEXT_DISPENSE_DATE") {
        return FoodDispenser::getInstance().getNextDispenseDate();
    } else if (var == "IS_DAILY") {
        return FoodDispenser::getInstance().isAlarmDaily() ? R"(<i class="fa fa-repeat" style="color: #1d6975"></i>)"
                                                           : "";
    }

    return "-";
}
