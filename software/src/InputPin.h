#pragma once

#include <Arduino.h>
#include "Pin.h"

class InputPin : public Pin {
public:
    explicit InputPin(int _pin): Pin(_pin) {}

    void initPin() const override {
        pinMode(pin, INPUT);
    }

    int readRaw() const {
        return analogRead(pin);
    }

    int readDigital() const {
        return digitalRead(pin);
    }
};