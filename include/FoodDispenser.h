//
// Created by Norbi on 2020. 12. 18..
//

#ifndef CATFEEDER_FOODDISPENSER_H
#define CATFEEDER_FOODDISPENSER_H


#include <Motor.h>
#include <TimeAlarms.h>

struct FoodAlarm {
    AlarmID_t id = 0;
    bool isSet = false;
    bool isDaily = false;
    int hour = -1, minute = -1;
    OnTick_t callback = nullptr;
};

class FoodDispenser {
public:

    static FoodDispenser &getInstance();

    void addFoodAlarm(int hour, int minute, bool isDaily);

    void processAlarm();

    void giveFood();

    void addMotorAction(MotorAction action);

    void clearMotorActions();

    void setMotorPin(int pin);

    String getLastDispenseDate() const;

    String getLastDispenseTime() const;

    String getNextDispenseDate() const;

    String getNextDispenseTime() const;

private:
    FoodAlarm alarm;
    Motor motor;
    TimeElements lastDispenseDate{};

    bool hasPrevious = false;

    FoodDispenser();
};


#endif //CATFEEDER_FOODDISPENSER_H
