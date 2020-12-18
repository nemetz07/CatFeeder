//
// Created by Norbi on 2020. 12. 18..
//

#include "FoodDispenser.h"

void foodAlarmCallback() {
    FoodDispenser::getInstance().processAlarm();
}

FoodDispenser &FoodDispenser::getInstance() {
    static FoodDispenser instance;

    return instance;
}

void FoodDispenser::giveFood() {
    if (!hasPrevious) {
        hasPrevious = true;
    }

    TimeElements elements;
    breakTime(now(), elements);
    lastDispenseDate = elements;

    motor.start();
}

void FoodDispenser::addFoodAlarm(int hour, int minute, bool isDaily) {
    if (alarm.isSet) {
        Alarm.disable(alarm.id);
        Alarm.free(alarm.id);
    }

    AlarmID_t alarmId = isDaily ? Alarm.alarmRepeat(hour, minute, 0, foodAlarmCallback) : Alarm.alarmOnce(hour, minute,
                                                                                                          0,
                                                                                                          foodAlarmCallback);

    FoodAlarm newAlarm;
    newAlarm.id = alarmId;
    newAlarm.isDaily = isDaily;
    newAlarm.isSet = true;
    newAlarm.hour = hour;
    newAlarm.minute = minute;
    newAlarm.callback = foodAlarmCallback;

    this->alarm = newAlarm;
}

FoodDispenser::FoodDispenser() {
    MotorAction action1(0, 90, 2000);
    MotorAction action2(90, 0, 2000);
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
        Alarm.disable(alarm.id);
        Alarm.free(alarm.id);
        alarm.isSet = false;
    }

    giveFood();
}
