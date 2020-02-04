#include "Driver.h"

#include <Arduino.h>

void Driver::drive(int speedLeft, int speedRight) {
    left.move(speedLeft);
    right.move(speedRight);
}

void Driver::pwmDrive(int speedLeft, int speedRight, int timeDrive, int timeWait) {
    drive(speedLeft, speedRight);
    delay(timeDrive);
    stop();
    delay(timeWait);
}

void Driver::forward() {
    pwmDrive(DEFAULT_SPEED_FORWARD, DEFAULT_SPEED_FORWARD, DEFAULT_TIME_DRIVE, DEFAULT_TIME_WAIT);
}

void Driver::turnLeft() {
    pwmDrive(-DEFAULT_SPEED_TURN, DEFAULT_SPEED_TURN, DEFAULT_TIME_DRIVE, DEFAULT_TIME_WAIT);
}

void Driver::turnRight() {
    pwmDrive(DEFAULT_SPEED_TURN, -DEFAULT_SPEED_TURN, DEFAULT_TIME_DRIVE, DEFAULT_TIME_WAIT);
}

void Driver::stop() {
    left.stop();
    right.stop();
}
