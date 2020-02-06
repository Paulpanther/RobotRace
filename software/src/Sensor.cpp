#include "Sensor.h"

#include <Arduino.h>

Sensor::Sensor(int _pin): InputPin(_pin) {
    reset();
}

void Sensor::calibrate() {
    int localMax[BUCKETS_COUNT / 2] = {0};
    int currentLocalMaxIndex = 0;

    //save all local maxima in localMax[]
    for (int i = 1; i < BUCKETS_COUNT - 1; i++){
        if (buckets[i-1] < buckets[i] && buckets[i] > buckets[i+1]){
            localMax[currentLocalMaxIndex] = i;
            currentLocalMaxIndex ++;
        }
    }

    int max1 = 0, max2 = 0, max3 = 0;
    for (int i : localMax) {
        if (buckets[i] > buckets[max1]) {
            max3 = max2;
            max2 = max1;
            max1 = i;
        } else if (buckets[i] > buckets[max2]) {
            max3 = max2;
            max2 = i;
        } else if (buckets[i] > buckets[max3]) {
            max3 = i;
        }
    }

    white = min(max1, min(max2, max3));
    black = max(max1, max(max2, max3));
    grey = max1 + max2 + max3 - black - white;

    //compute thresholds
    black_grey_thresh = (black + grey) / 2;
    white_grey_thresh = (white + grey) / 2;

}

Color Sensor::decode(int value) {
    int bucketIndex = value * BUCKETS_COUNT / MAX_VALUE;
    if (bucketIndex >= BUCKETS_COUNT || bucketIndex < 0)
        return BLACK;

    buckets[bucketIndex]++;

    calibrate();

//    Serial.print(black_grey_thresh);
//    Serial.print(" ");
//    Serial.print(white_grey_thresh);
//    Serial.print(" ");
//    Serial.print(bucketIndex);
//    Serial.print(" ");

    if (bucketIndex < white_grey_thresh)
        return WHITE;
    else if (bucketIndex < black_grey_thresh)
        return GREY;
    else
        return BLACK;
}

Color Sensor::read() {
    int raw = readRaw();
    lastValues = lastValues * 0.99f + 0.01f * ((float) decode(raw));
    return (Color) (int) lround((double) lastValues);
//    return decode(raw);
}

void Sensor::reset() {
    for (int & bucket : buckets) {
        bucket = 0;
    }
}
