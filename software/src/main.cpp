#include <Arduino.h>
#include "Sensor.h"
#include "Driver.h"

#define DEBUG
// #define DRIVE_STRAIGHT

Sensor sensorFront(A0);
Sensor sensorLeft(A2);
Sensor sensorRight(A1);

Driver drive(10, 9, 5, 6);

void setup() {
    sensorFront.initPin();
    sensorLeft.initPin();
    sensorRight.initPin();
    drive.initPins();
    drive.getMotorLeft().setNegativeOffset(0);
    drive.getMotorRight().setNegativeOffset(0);

    Serial.begin(115200);
}

boolean pointsToGoal(Color front, Color back) {
    return front == back ||
        (front == WHITE && back == GREY) ||
        (front == GREY && back == BLACK) ||
        (front == BLACK && back == WHITE);
}

// TODO after driving forward for n time, rotate a bit
void move(Color front, Color left, Color right) {
    if (front != left && left != right && front != right) {
        // Ignore
        drive.forward();
    } else if (front == left && left == right) {
        // All same
        drive.forward();
    } else if (left == right) {
        Color back = left;
        if (pointsToGoal(front, back)) {
            drive.forward();
        } else {
            // Turn 180
            drive.turnRight();
        }
    } else {
        boolean isLeftSame = front == left;
        Color other = isLeftSame ? right : left;

        if (pointsToGoal(other, front)) {
            isLeftSame ? drive.turnRight() : drive.turnLeft();
        } else {
            isLeftSame ? drive.turnLeft() : drive.turnRight();
        }
    }
}

char buffer[20];

void printSensorReadings(Color front, Color left, Color right) {
    String out = "%i %i %i";
    sprintf(buffer, "%u %u %u", front, left, right);
    Serial.println(buffer);
}

void loop() {
    Color front = sensorFront.read();
    Color left = sensorLeft.read();
    Color right = sensorRight.read();

#if defined(DEBUG)
    printSensorReadings(front, left, right);
#endif

#if defined(DRIVE_STRAIGHT)
    drive.forward();
#else
    move(front, left, right);
#endif
}
