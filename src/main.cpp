#include <Arduino.h>
#include <WiFi.h>
#include "TimeUtil.h"

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
}

void loop() {

}