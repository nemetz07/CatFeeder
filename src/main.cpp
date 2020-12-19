#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <controller/HomeController.h>
#include <SPIFFS.h>
#include <controller/SettingsController.h>
#include <TimeAlarms.h>
#include <controller/ScheduleController.h>
#include <FoodDispenser.h>
#include "TimeUtil.h"
#include "Constants.h"
#include "ArduinoJson.h"

AsyncWebServer server(SERVER_PORT);

void setupSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
}

void setupWifi() {
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());
}

void setupTimeSync() {
    TimeUtil::init();
    Serial.println("Current time: ");
    TimeUtil::digitalClockDisplay();
}

void setupRoutes() {
    //Static files
    server.serveStatic("/sb-admin.css", SPIFFS, "/sb-admin.css");
//    server.serveStatic("/bootstrap.min.css", SPIFFS, "/bootstrap.min.css");
//    server.serveStatic("/bootstrap.min.js", SPIFFS, "/bootstrap.min.js");
    server.serveStatic("/sb-admin.js", SPIFFS, "/sb-admin.js");
    server.serveStatic("/jquery-3.5.1.min.js", SPIFFS, "/jquery-3.5.1.min.js");
    server.serveStatic("jquery.timepicker.min.css", SPIFFS, "jquery.timepicker.min.css");
    server.serveStatic("jquery.timepicker.min.js", SPIFFS, "jquery.timepicker.min.js");

    //View routes
    server.on("/", HTTP_GET, HomeController::index);
    server.on("/settings", HTTP_GET, SettingsController::index);
    server.on("/schedule", HTTP_GET, ScheduleController::index);

    //API routes
    server.on("/api/servoPin", HTTP_POST, SettingsController::setServoPin);
    server.on("/api/testMotor", HTTP_POST, SettingsController::testMotor);
    server.on("/api/schedule", HTTP_POST, ScheduleController::setSchedule);
    server.on("/api/schedule", HTTP_DELETE, ScheduleController::deleteSchedule);
}

void setupServer() {
    Serial.println("Setting up server");
    setupRoutes();

    server.begin();

    Serial.print("Started server at: ");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.println(SERVER_PORT);
}

void setup() {
    Serial.begin(115200);
    delay(250);

    setupSPIFFS();
    setupWifi();
    setupTimeSync();
    setupServer();

    FoodDispenser::getInstance().init();
}

void loop() {
    Alarm.delay(10000);
}