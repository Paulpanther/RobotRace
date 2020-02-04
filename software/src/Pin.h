#pragma once

class Pin {
public:
    const int pin;

    explicit Pin(int _pin): pin(_pin) {}
    virtual void initPin() const {};
};


