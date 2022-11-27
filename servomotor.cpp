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
    this->angle = clamp(this->angle, 0, 180);

    Serial.println("Servo " + String(this->pin) + " moving to: " + String(this->angle));

    // This check prevents servos that aren't moving from shaking as much
    if (!abs(this->angle - this->targetAngle) < 0.001f) {
        this->motor.detach();
        this->motor.setPeriodHertz(this->hertz);
        this->motor.attach(this->pin, this->min, this->max);
        this->motor.write(this->angle);
    }
}