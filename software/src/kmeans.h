#pragma once

#define SAMPLE_LENGTH 120


float mean(float data[], int len);

float stddev(float data[], int len);

void thresholding(float y[], int signals[], int lag, float threshold, float influence);
