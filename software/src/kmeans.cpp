#include "kmeans.h"
#include <Arduino.h>

float mean(float data[], int len) {
    float sum = 0.0, mean = 0.0;

    int i;
    for(i=0; i<len; ++i) {
//        sum += data[i];
    }
//    mean = sum/len;
    return mean;


}

float stddev(float data[], int len) {
    float the_mean = mean(data, len);
    float standardDeviation = 0.0;

    int i;
    for(i=0; i<len; ++i) {
        standardDeviation += pow(data[i] - the_mean, 2);
    }

    return sqrt(standardDeviation/len);
}

void thresholding(float y[], int signals[], int lag, float threshold, float influence) {
    memset(signals, 0, sizeof(int) * SAMPLE_LENGTH);
    float filteredY[SAMPLE_LENGTH] = {0};
//    memcpy(filteredY, y, sizeof(float) * SAMPLE_LENGTH);
    for (int i = 0; i < SAMPLE_LENGTH; i++) {
        filteredY[i] = y[i];
        Serial.println(filteredY[i]);
    }
    float avgFilter[SAMPLE_LENGTH];
    float stdFilter[SAMPLE_LENGTH];

    avgFilter[lag - 1] = mean(y, lag);
    stdFilter[lag - 1] = stddev(y, lag);

    for (int i = lag; i < SAMPLE_LENGTH; i++) {
        if (fabsf(y[i] - avgFilter[i-1]) > threshold * stdFilter[i-1]) {
            if (y[i] > avgFilter[i-1]) {
                signals[i] = 1;
            } else {
                signals[i] = -1;
            }
            filteredY[i] = influence * y[i] + (1 - influence) * filteredY[i-1];
        } else {
            signals[i] = 0;
        }
//        avgFilter[i] = mean(filteredY + i-lag, lag);
//        stdFilter[i] = stddev(filteredY + i-lag, lag);
    }
}

