#pragma once

#include "InputPin.h"

#define BUCKETS_COUNT 120
#define MAX_VALUE 600

enum Color {
    WHITE,
    GREY,
    BLACK
};

class Sensor : public InputPin {
private:
    int black = 0, grey = 0, white = 0;
    int black_grey_thresh = 0;
    int white_grey_thresh = 0;

    int buckets[BUCKETS_COUNT];

//    History history;
    float lastValues = 1;

    void calibrate();
    Color decode(int value);
public:
    explicit Sensor(int _pin);

    Color read();

    void reset();
};


