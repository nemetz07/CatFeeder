//
// Created by Norbi on 2020. 12. 16..
//

#include "Motor.h"

Motor::Motor() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    servo.setPeriodHertz(DEFAULT_PERIOD_HERTZ);
    servo.attach(pin, min, max);

}

void Motor::reset() {
    servo.detach();
    servo.setPeriodHertz(periodHertz);
    servo.attach(pin, min, max);
}

void Motor::start() {
    Serial.println("Executing actions!");
    for (int i = 0; i < actionLength; ++i) {
        doAction(actions[i]);
    }
}

void Motor::doAction(MotorAction action) {
    Serial.print("ACTION - ");

    Serial.print("Setting position: ");
    Serial.print(action.startDegree);
    servo.write(action.startDegree);

    Serial.print(" degree | Waiting for: ");
    Serial.print(action.startDelay);
    delay(action.startDelay);

    Serial.print(" ms | Setting position: ");
    Serial.print(action.endDegree);
    servo.write(action.endDegree);

    Serial.print(" degree | Waiting for: ");
    Serial.print(action.endDelay);
    Serial.println(" ms");
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

int Motor::getPin() const {
    return pin;
}

void Motor::setPin(int pin) {
    Motor::pin = pin;
    reset();
}

void Motor::turnDegree(int value) {
    servo.write(value);
}

int Motor::getMin() const {
    return min;
}

void Motor::setMin(int min) {
    Motor::min = min;

    reset();
}

int Motor::getMax() const {
    return max;
}

void Motor::setMax(int max) {
    Motor::max = max;

    reset();
}

int Motor::getPeriodHertz() const {
    return periodHertz;
}

void Motor::setPeriodHertz(int periodHertz) {
    Motor::periodHertz = periodHertz;

    reset();
}

MotorAction::MotorAction(int startDegree, int endDegree, int startDelay, int endDelay) : startDegree(startDegree),
                                                                                         endDegree(endDegree),
                                                                                         startDelay(startDelay),
                                                                                         endDelay(endDelay) {}

MotorAction::MotorAction(int startDegree, int endDegree, int startDelay) : MotorAction(startDegree, endDegree,
                                                                                       startDelay, -1) {}

MotorAction::MotorAction() : MotorAction(0, 0, 0, 0) {}

