//
// Created by Norbi on 2020. 12. 17..
//

#include <SPIFFS.h>
#include <FoodDispenser.h>
#include "controller/HomeController.h"

void HomeController::index(AsyncWebServerRequest *request) {
    Serial.println("Home - index");
    request->send(SPIFFS, "/index.html", String(), false, processor);
}

String HomeController::processor(const String &var) {
    if (var == "TITLE") {
        return "CatFeeder";
    } else if (var == "LAST_DISPENSE_TIME") {
        return FoodDispenser::getInstance().getLastDispenseTime();
    } else if (var == "LAST_DISPENSE_DATE") {
        return FoodDispenser::getInstance().getLastDispenseDate();
    } else if (var == "NEXT_DISPENSE_TIME") {
        return FoodDispenser::getInstance().getNextDispenseTime();
    } else if (var == "NEXT_DISPENSE_DATE") {
        return FoodDispenser::getInstance().getNextDispenseDate();
    }

    return "-";
}
