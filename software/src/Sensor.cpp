#include "Sensor.h"

#include <Arduino.h>

Sensor::Sensor(int _pin): InputPin(_pin) {
    reset();
}

void Sensor::calibrate() {
    int localMax[BUCKETS_COUNT / 2] = {0};
    int currentLocalMaxIndex = 0;
    int lastFoundMaxIndex = -MIN_BUCKET_DIST;

    //save all local maxima in localMax[]
    for (int i = 1; i < BUCKETS_COUNT - 1; i++) {
        if (buckets[i-1] < buckets[i] && buckets[i] > buckets[i+1]
                && i - lastFoundMaxIndex >= MIN_BUCKET_DIST){
            localMax[currentLocalMaxIndex] = i;
            currentLocalMaxIndex ++;
            lastFoundMaxIndex = i;
        }
    }

    int max1 = 0;
    for (int i : localMax) {
        if (buckets[i] > buckets[max1])
            max1 = i;
    }

    int max2 = 0;
    for (int i : localMax) {
        if (buckets[i] > buckets[max2] && abs(i - max1) >= MIN_BUCKET_DIST)
            max2 = i;
    }

    int max3 = 0;
    for (int i : localMax) {
        if (buckets[i] > buckets[max3]
                && abs(i - max1) >= MIN_BUCKET_DIST
                && abs(i - max2) >= MIN_BUCKET_DIST)
        max3 = i;
    }

    white = min(max1, min(max2, max3));
    black = max(max1, max(max2, max3));
    grey = max1 + max2 + max3 - black - white;

    //compute thresholds
    black_grey_thresh = (black + grey) / 2;
    white_grey_thresh = (white + grey) / 2;

}

Color Sensor::decode(int value) {
    int bucketIndex = (int) (((uint16_t) value) * BUCKETS_COUNT / MAX_VALUE);
    if (bucketIndex >= BUCKETS_COUNT || bucketIndex < 0) {
        return BLACK;
    }

    buckets[bucketIndex]++;

    calibrate();

//    Serial.print(black_grey_thresh);
//    Serial.print(" ");
//    Serial.print(white_grey_thresh);
//    Serial.print(" ");
//    Serial.print(bucketIndex);
//    Serial.print(" ");
//    Serial.print(value);
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
    int decoded = decode(raw);
    lastValues = lastValues * 0.95f + 0.05f * ((float) decoded);
    return (Color) (int) lround((double) lastValues);
//    return (Color) decoded;
}

void Sensor::reset() {
    for (uint32_t & bucket : buckets) {
        bucket = 0;
    }
}
