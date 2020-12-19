//
// Created by Norbi on 2020. 12. 18..
//

#ifndef CATFEEDER_FOODDISPENSER_H
#define CATFEEDER_FOODDISPENSER_H


#include <Motor.h>
#include <TimeAlarms.h>

#define ALARM_FILE "/alarm.json"

struct FoodAlarm {
    AlarmID_t id = 0;
    bool isSet = false;
    bool isDaily = false;
    int hour = -1, minute = -1, portion = 1;
};

class FoodDispenser {
public:

    static FoodDispenser &getInstance();

    void addAlarm(int hour, int minute, int portion, bool isDaily);

    void processAlarm();

    void giveFood();

    void addMotorAction(MotorAction action);

    void clearMotorActions();

    void setMotorPin(int pin);

    String getLastDispenseDate() const;

    String getLastDispenseTime() const;

    String getNextDispenseDate() const;

    String getNextDispenseTime() const;

    void deleteAlarm(bool withFile);

    bool isAlarmDaily() const;

    void init();

private:
    FoodAlarm alarm;
    Motor motor;
    TimeElements lastDispenseDate{};

    bool hasPrevious = false;

    void saveAlarmToFile() const;

    void loadAlarmFromFile();

    void deleteAlarmFile();

    void setAlarm(int hour, int minute, int portion, bool isDaily);

    FoodDispenser();
};


#endif //CATFEEDER_FOODDISPENSER_H
