#include <Arduino.h>
#include <WiFi.h>
#include "TimeUtil.h"
#include "Motor.h"

Motor motor = Motor();

void setup() {
    Serial.begin(115200);
    delay(250);

    Serial.println("TimeNTP Example");
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting UDP");
    TimeUtil::init();
    Serial.println("Current time: ");
    TimeUtil::digitalClockDisplay();

    MotorAction action1(0,90,1000);
    MotorAction action2(90,180,1000);
    MotorAction action3(180,90,1000);
    MotorAction action4(90,0,1000);

    motor.addAction(action1);
    motor.addAction(action2);
    motor.addAction(action3);
    motor.addAction(action4);
}

void loop() {
    motor.start();
}