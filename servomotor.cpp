/*
    Servo Motor Class
*/

#include "servomotor.h"
#include "helper.h"
#include <ESP32Servo.h>

ServoMotor::ServoMotor(float angle, int maxAngle, int pin, int min, int max, int hertz) : Motor(angle) 
{
    this->pin = pin;
    this->min = min;
    this->max = max;
    this->hertz = hertz;
    this->maxAngle = maxAngle;
}

void ServoMotor::move(float progress)
{
    float lastAngle = this->angle;

    Motor::move(progress);

    // Clamp between min and max angle of servo
    this->angle = clamp(this->angle, 0, this->maxAngle);

    // Write to servo
    this->motor.detach();
    this->motor.setPeriodHertz(this->hertz);
    this->motor.attach(this->pin, this->min, this->max);
    this->motor.write(this->angle);
}