#pragma once

#include "Motor.h"

#define DEFAULT_TIME_DRIVE 5
#define DEFAULT_TIME_WAIT 3

#define DEFAULT_SPEED_FORWARD 255
#define DEFAULT_SPEED_TURN 255

class Driver {
private:
    Motor left, right;

public:
    Driver(int pinLeftForwards, int pinLeftBackwards, int pinRightForwards, int pinRightBackwards)
    : left(pinLeftForwards, pinLeftBackwards), right(pinRightForwards, pinRightBackwards) {}

    void drive(int speedLeft, int speedRight);
    void pwmDrive(int speedLeft, int speedRight, int timeDrive, int timeWait);
    void forward();
    void turnLeft();
    void turnRight();
    void stop();

    void initPins() {
        left.initPins();
        right.initPins();
    }

    Motor & getMotorLeft() {
        return left;
    }

    Motor & getMotorRight() {
        return right;
    }
};


