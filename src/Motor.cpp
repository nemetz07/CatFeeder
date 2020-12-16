//
// Created by Norbi on 2020. 12. 16..
//

#include "Motor.h"

Motor::Motor() : Motor(DEFAULT_PIN, DEFAULT_MIN, DEFAULT_MAX) {
}

Motor::Motor(int pin) : Motor(pin, DEFAULT_MIN, DEFAULT_MAX) {
}

Motor::Motor(int pin, int min, int max) {
    this->pin = pin;
    this->min = min;
    this->max = max;

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    servo.setPeriodHertz(DEFAULT_PERIOD_HERTZ);
    servo.attach(pin, min, max);
}

void Motor::setPeriodHertz(int hertz) {
    this->periodHertz = hertz;

    reset();
}

void Motor::reset() {
    servo.setPeriodHertz(periodHertz);
    servo.detach();
    servo.attach(pin, min, max);
}

void Motor::start() {
    for (int i = 0; i < actionLength; ++i) {
        doAction(actions[i]);
    }
}

void Motor::doAction(MotorAction action) {
    servo.write(action.startDegree);
    delay(action.startDelay);
    servo.write(action.endDegree);
    delay(action.endDelay);
}

void Motor::clearActions() {
    actionLength = 0;
}

void Motor::addAction(MotorAction action) {
    if (action.startDelay < DELAY_MIN) {
        action.startDelay = DELAY_MIN;
    }
    if (action.startDelay > DELAY_MAX) {
        action.startDelay = DELAY_MAX;
    }
    if (action.endDelay < 0) {
        action.endDelay = action.startDelay;
    }
    if (action.endDelay < DELAY_MIN) {
        action.endDelay = DELAY_MIN;
    }
    if (action.endDelay > DELAY_MAX) {
        action.endDelay = DELAY_MAX;
    }

    actions[actionLength] = action;
    actionLength++;
}

MotorAction::MotorAction(int startDegree, int endDegree, int startDelay, int endDelay) : startDegree(startDegree),
                                                                                         endDegree(endDegree),
                                                                                         startDelay(startDelay),
                                                                                         endDelay(endDelay) {}

MotorAction::MotorAction(int startDegree, int endDegree, int startDelay) : MotorAction(startDegree, endDegree,
                                                                                       startDelay, -1) {}

MotorAction::MotorAction(): MotorAction(0,0,0,0) {}
