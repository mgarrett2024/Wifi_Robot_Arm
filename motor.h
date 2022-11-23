#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
protected:
    float startAngle;
    float targetAngle;
    float angle;
    float smoothness = 1.5f;
    
public:
    Motor(float angle);
    Motor() = default;

    void set_angle(float targetAngle);
    void move(float progress);
};

#endif