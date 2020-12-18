//
// Created by Norbi on 2020. 12. 18..
//

#include <FoodDispenser.h>
#include "controller/ScheduleController.h"

void ScheduleController::addAlarm(AsyncWebServerRequest *request) {
    Serial.println("ScheduleController - add alarm");

    if (!request->hasArg("hour")) {
        Serial.println("No hour param!");
        request->send(400, "text/plain", "Error: param 'hour' is missing");
        return;
    }

    int hour = (request->arg("hour")).toInt();
    int minute = (request->arg("minute")).toInt();
    bool isDaily = (request->arg("isDaily")) == "true";

    Serial.println("Params: ");
    Serial.println(hour);
    Serial.println(minute);
    Serial.println(isDaily);

    FoodDispenser::getInstance().addFoodAlarm(hour, minute, isDaily);
    request->send(200, "text/plain", "success");
}
