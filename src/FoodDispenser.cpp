//
// Created by Norbi on 2020. 12. 18..
//

#include <SPIFFS.h>
#include <TimeUtil.h>
#include "FoodDispenser.h"
#include "ArduinoJson.h"

void foodAlarmCallback() {
    FoodDispenser::getInstance().processAlarm();
}

void cooldownCallback() {
    FoodDispenser::getInstance().resetCooldown();
}

FoodDispenser &FoodDispenser::getInstance() {
    static FoodDispenser instance;

    return instance;
}

void FoodDispenser::giveFood() {
    if (!hasPrevious) {
        hasPrevious = true;
    }

    breakTime(now(), lastDispenseDate);

    motor.start();
}

void FoodDispenser::addAlarm(int hour, int minute, int portion, bool isDaily) {
    if (alarm.isSet) {
        deleteAlarm(false);
    }

    setAlarm(hour, minute, portion, isDaily);
    saveAlarmToFile();
}

FoodDispenser::FoodDispenser() {
    MotorAction action1(0, 180, 1000);
    MotorAction action2(180, 0, 1000);
    motor.addAction(action1);
    motor.addAction(action2);
}

void FoodDispenser::addMotorAction(MotorAction action) {
    motor.addAction(action);
}

void FoodDispenser::clearMotorActions() {
    motor.clearActions();
}

void FoodDispenser::setMotorPin(int pin) {
    motor.setPin(pin);
}

String FoodDispenser::getLastDispenseDate() const {
    if (hasPrevious) {
        String date(lastDispenseDate.Year + 1970);
        date.concat(".");
        date.concat(lastDispenseDate.Month);
        date.concat(".");
        date.concat(lastDispenseDate.Day);
        return date;
    } else {
        return "";
    }
}

String FoodDispenser::getLastDispenseTime() const {
    if (hasPrevious) {
        String time(lastDispenseDate.Hour);
        lastDispenseDate.Minute < 10 ? time.concat(":0") : time.concat(":");
        time.concat(lastDispenseDate.Minute);
        return time;
    } else {
        return "-";
    }
}

String FoodDispenser::getNextDispenseDate() const {
    if (alarm.isSet) {
        String date(year());
        date.concat(".");
        date.concat(month());
        date.concat(".");

        if (alarm.hour < hour() || (alarm.hour == hour() && alarm.minute <= minute())) {
            date.concat(day() + 1);
            return date;
        } else {
            date.concat(day());
        }
        return date;
    } else {
        return "";
    }
}

String FoodDispenser::getNextDispenseTime() const {
    if (alarm.isSet) {
        String time(alarm.hour);
        alarm.minute < 10 ? time.concat(":0") : time.concat(":");
        time.concat(alarm.minute);
        return time;
    } else {
        return "-";
    }
}

void FoodDispenser::processAlarm() {
    if (!alarm.isDaily) {
        deleteAlarm(true);
    }

    for (int i = 0; i < alarm.portion; ++i) {
        giveFood();
    }
}

bool FoodDispenser::isAlarmDaily() const {
    return alarm.isSet && alarm.isDaily;
}

void FoodDispenser::deleteAlarm(bool withFile) {
    Alarm.free(alarm.id);
    alarm.isSet = false;

    if (withFile) {
        deleteAlarmFile();
    }
}

void FoodDispenser::loadAlarmFromFile() {
    Serial.println("Loading alarm");
    Serial.print("Opening file for reading - ");
    Serial.println(ALARM_FILE);

    File file = SPIFFS.open(ALARM_FILE, FILE_READ);
    if (!file) {
        Serial.println("There was an error opening the file for writing");
        return;
    }

    StaticJsonDocument<128> doc;

    Serial.println("Deserializing from file...");
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
    }

    Serial.println("Closing file");
    file.close();

    int hour = doc["hour"] | 12;
    int minute = doc["minute"] | 0;
    int portion = doc["portion"] | 1;
    bool isDaily = doc["isDaily"] | false;

    setAlarm(hour, minute, portion, isDaily);
}

void FoodDispenser::saveAlarmToFile() const {
    Serial.println("Saving alarm");
    Serial.print("Opening file for writing - ");
    Serial.println(ALARM_FILE);

    File file = SPIFFS.open(ALARM_FILE, FILE_WRITE);
    if (!file) {
        Serial.println("There was an error opening the file for writing");
        return;
    }

    StaticJsonDocument<128> doc;
    doc["hour"] = alarm.hour;
    doc["minute"] = alarm.minute;
    doc["portion"] = alarm.portion;
    doc["isDaily"] = alarm.isDaily;

    Serial.println("Writing alarm to file...");
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }

    Serial.println("Closing file");
    file.close();
}

void FoodDispenser::deleteAlarmFile() {
    if (SPIFFS.exists(ALARM_FILE)) {
        Serial.println("Deleting alarm file");
        SPIFFS.remove(ALARM_FILE);
    }
}

void FoodDispenser::init() {
    if (SPIFFS.exists(ALARM_FILE)) {
        loadAlarmFromFile();
    }
    if (SPIFFS.exists(COOLDOWN_FILE)) {
        loadCooldownFromFile();
    }
}

void FoodDispenser::setAlarm(int hour, int minute, int portion, bool isDaily) {
    AlarmID_t alarmId = isDaily ? Alarm.alarmRepeat(hour, minute, 0, foodAlarmCallback) : Alarm.alarmOnce(hour, minute,
                                                                                                          0,
                                                                                                          foodAlarmCallback);
    Serial.print("Set alarm for ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(" | Portion: ");
    Serial.print(portion);
    if (isDaily) {
        Serial.print(" (Repeat)");
    }
    Serial.println();

    alarm.id = alarmId;
    alarm.isDaily = isDaily;
    alarm.isSet = true;
    alarm.hour = hour;
    alarm.minute = minute;
    alarm.portion = (portion < 1 || portion > 5) ? 1 : portion;
}

void FoodDispenser::giveFoodManual() {
    if (!manualReady) {
        Serial.print("Manual food dispensing is unavailable until ");
        Serial.println(TimeUtil::getDateTime(cooldownEnd));
        return;
    }

    Serial.println("Manual food dispensing");
    manualReady = false;
    breakTime(now() + COOLDOWN_TIME, cooldownEnd);
    cooldownAlarmId = Alarm.timerOnce(COOLDOWN_TIME, cooldownCallback);
    saveCooldownToFile();

    giveFood();
}

void FoodDispenser::resetCooldown() {
    Serial.println("Manual is ready");
    Alarm.free(cooldownAlarmId);

    manualReady = true;
    cooldownEnd = {};
    cooldownAlarmId = 0;
    deleteCooldownFile();
}

const TimeElements &FoodDispenser::getCooldownEnd() const {
    return cooldownEnd;
}

bool FoodDispenser::isManualReady() const {
    return manualReady;
}

void FoodDispenser::deleteCooldownFile() {
    if (SPIFFS.exists(COOLDOWN_FILE)) {
        Serial.println("Deleting cooldown file");
        SPIFFS.remove(COOLDOWN_FILE);
    }
}

void FoodDispenser::saveCooldownToFile() {
    Serial.println("Saving cooldown");
    Serial.print("Opening file for writing - ");
    Serial.println(COOLDOWN_FILE);

    File file = SPIFFS.open(COOLDOWN_FILE, FILE_WRITE);
    if (!file) {
        Serial.println("There was an error opening the file for writing");
        return;
    }

    StaticJsonDocument<64> doc;
    doc["timestamp"] = makeTime(cooldownEnd);

    Serial.println("Writing cooldown to file...");
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }

    Serial.println("Closing file");
    file.close();
}

void FoodDispenser::loadCooldownFromFile() {
    Serial.println("Loading cooldown");
    Serial.print("Opening file for reading - ");
    Serial.println(COOLDOWN_FILE);

    File file = SPIFFS.open(COOLDOWN_FILE, FILE_READ);
    if (!file) {
        Serial.println("There was an error opening the file for writing");
        return;
    }

    StaticJsonDocument<64> doc;

    Serial.println("Deserializing from file...");
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
    }

    Serial.println("Closing file");
    file.close();

    time_t timestamp = doc["timestamp"] | -1;

    if (timestamp != -1) {
        manualReady = false;

        cooldownAlarmId = Alarm.timerOnce(timestamp - now(), cooldownCallback);
        breakTime(timestamp, cooldownEnd);
    }
}
