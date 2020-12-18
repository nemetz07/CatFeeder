//
// Created by Norbi on 2020. 12. 16..
//

#ifndef CATFEEDER_MOTOR_H
#define CATFEEDER_MOTOR_H

#include <ESP32Servo.h>

#define DEFAULT_PERIOD_HERTZ 50
#define DEFAULT_MIN 500
#define DEFAULT_MAX 2500
#define DEFAULT_PIN 32
#define ACTIONS_MAX 5
#define DELAY_MIN 100
#define DELAY_MAX 5000


struct MotorAction {
public:
    int startDegree = 0;
    int endDegree = 0;
    int startDelay = 1000;
    int endDelay = -1;

    MotorAction();

    MotorAction(int startDegree, int endDegree, int startDelay);

    MotorAction(int startDegree, int endDegree, int startDelay, int endDelay);
};

class Motor {
public:
    void reset();

    void start();

    void addAction(MotorAction action);

    void clearActions();

    void turnDegree(int value);

    int getPin() const;

    void setPin(int pin);

    int getMin() const;

    void setMin(int min);

    int getMax() const;

    void setMax(int max);

    int getPeriodHertz() const;

    void setPeriodHertz(int periodHertz);

    Motor();

private:
    Servo servo;

    int pin = DEFAULT_PIN;
    int min = DEFAULT_MIN;
    int max = DEFAULT_MAX;
    int periodHertz = DEFAULT_PERIOD_HERTZ;

    MotorAction actions[ACTIONS_MAX];

    int actionLength = 0;

    void doAction(MotorAction action);
};

#endif //CATFEEDER_MOTOR_H
