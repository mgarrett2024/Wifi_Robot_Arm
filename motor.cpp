/*
    Base Motor Class
*/

#include "motor.h"
#include "helper.h"

// Constructors

// Constructor
Motor::Motor(float angle)
{
    this->startAngle = angle;
    this->angle = angle;
}

// Motor Functions

// Set motor's target angle
void Motor::set_angle(float targetAngle)
{
    this->targetAngle = targetAngle;      // Set motor's target angle
    this->startAngle = this->angle; // Set motor's starting angle
}

// Lerp motor's angle to smoothly over progress
void Motor::move(float progress)
{
    if (progress >= 1)
    {
        this->angle = this->targetAngle;
    }
    else
    {
        this->angle = lerp(
            this->startAngle,
            this->targetAngle,
            q_lerp(progress, this->smoothness));
    }
}