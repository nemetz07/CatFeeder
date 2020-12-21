//
// Created by Norbi on 2020. 12. 17..
//

#include <SPIFFS.h>
#include <Motor.h>
#include <FoodDispenser.h>
#include <TimeUtil.h>
#include <Constants.h>
#include "controller/SettingsController.h"
#include "ArduinoJson.h"

void SettingsController::index(AsyncWebServerRequest *request) {
    Serial.println("Settings - index");
    request->send(SPIFFS, SETTINGS_HTML, String(), false, processor);
}

String SettingsController::processor(const String &var) {
    if (var == "TITLE") {
        return TITLE;
    } else if (var == "YEAR") {
        return YEAR;
    } else if (var == "PAGE_TITLE") {
        return "Beállítások";
    }

    return "-";
}

void SettingsController::setServoPin(AsyncWebServerRequest *request) {
//    Serial.println("Settings - change servo pin");
//    int newPin = (request->arg("value")).toInt();
//
//    Serial.print("Current pin: ");
//    Serial.println(Motor::getInstance().getPin());
//
//    Serial.print("New pin: ");
//    Serial.println(newPin);
//
//    Motor::getInstance().setPin(newPin);
    request->send(SPIFFS, SETTINGS_HTML, String(), false, processor);
}

void SettingsController::testMotor(AsyncWebServerRequest *request) {
//    Serial.println("Settings - test motor actions");
//    Serial.print("Degree: ");
//    int degree = (request->arg("value")).toInt();
//    Serial.println(degree);
//    Motor::getInstance().turnDegree(degree);

    request->send(200, "text/plain", "success");
}

void SettingsController::giveFoodManual(AsyncWebServerRequest *request) {
    StaticJsonDocument<128> doc;
    int code = 200;
    String message;

    if (FoodDispenser::getInstance().isManualReady()) {
        doc["message"] = "OK";
    } else {
        code = 400;
        String error("Manual food dispensing is unavailable until ");
        error.concat(TimeUtil::getDateTime(FoodDispenser::getInstance().getCooldownEnd()));
        doc["message"] = error;
    }

    ArduinoJson::serializeJson(doc, message);
    request->send(code, "application/json", message);

    FoodDispenser::getInstance().giveFoodManual();
}

void SettingsController::resetCooldown(AsyncWebServerRequest *request) {
    FoodDispenser::getInstance().resetCooldown();
}

