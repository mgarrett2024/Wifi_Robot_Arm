#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "motor.h"

class Stepper : public Motor
{
protected:
    int resolution = 200;
    int stepsTaken = 0;
    int totalSteps = 0;
    int DIR;
    int STEP;

public:
    // Constructor
    Stepper(int angle, int dirPin, int stepPin, int resolution);
    Stepper() = default;

    void set_angle(float angle);
    void move(float progress);
};

#endif