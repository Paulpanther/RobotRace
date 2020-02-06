#include "Motor.h"

#include <Arduino.h>

void Motor::move(int speed) {
    int actualSpeed = abs(speed);
    boolean driveForwards = speed > 0;

    if (actualSpeed > MOTOR_MIN) {
        actualSpeed -= offset;
    }


    actualSpeed = max(0, min(actualSpeed, MOTOR_MAX));  // Clamp value between 0 and 255

    if (driveForwards) {
        forwards.writeRaw(actualSpeed);
        backwards.writeRaw(0);
    } else {
        forwards.writeRaw(0);
        backwards.writeRaw(actualSpeed);
    }
}

void Motor::stop() {
    forwards.writeRaw(0);
    backwards.writeRaw(0);
}
