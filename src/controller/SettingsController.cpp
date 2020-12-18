//
// Created by Norbi on 2020. 12. 17..
//

#include <SPIFFS.h>
#include <Motor.h>
#include "controller/SettingsController.h"

void SettingsController::index(AsyncWebServerRequest *request) {
    Serial.println("Settings - index");
    request->send(SPIFFS, SETTINGS_HTML, String(), false, processor);
}

String SettingsController::processor(const String &var) {
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

