#pragma once

#include "OutputPin.h"

#define MOTOR_MIN 130
#define MOTOR_MAX 255

#define DEFAULT_PWM_FREQ 1

class Motor {
private:
    int offset = 0;
public:
    const OutputPin forwards, backwards;

    Motor(int _forwards, int _backwards): forwards(_forwards), backwards(_backwards) {}

    void initPins() const {
        forwards.initPin();
        backwards.initPin();
//        forwards.setPwmFrequency(1);
//        backwards.setPwmFrequency(1);
    }

    void setNegativeOffset(int _offset) {
        offset = _offset;
    }

    /**
     * @param speed Between 0 - 255
     */
    void move(int speed);
    void stop();
};


