#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Arduino.h>
#include "motor.h"
#include <ESP32Servo.h>

class ServoMotor : public Motor
{
protected:
    int pin;
    int min;
    int max;
    int hertz;
    int maxAngle;
    Servo motor;
public:
    // Constructor
    ServoMotor(float angle, int maxAngle, int pin, int min, int max, int hertz);
    ServoMotor() = default;

    void move(float progress = 1);
};

#endif