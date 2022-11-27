/*
    Stepper Class
*/

#include "stepper.h"
#include "helper.h"

Stepper::Stepper(int angle, int dirPin, int stepPin, int resolution) : Motor(angle)
{
    this->resolution = resolution;
    this->DIR = dirPin;
    this->STEP = stepPin;
    pinMode(this->STEP, OUTPUT);
    pinMode(this->DIR, OUTPUT);
}

void Stepper::set_angle(float targetAngle)
{
    // Use motor's function to set target angle
    Motor::set_angle(targetAngle);

    this->angle = fmod(this->angle, 360);
    this->targetAngle = fmod(this->targetAngle, 360);

    if (targetAngle - this->angle > 180)
    {
        this->targetAngle -= 360;
    }

    this->startAngle = this->angle;
    this->totalSteps = (this->targetAngle - this->startAngle) / 360.0f * this->resolution;
    this->stepsTaken = 0;
}

void Stepper::move(float progress)
{
    // Use motor's function to move
    Motor::move(progress);

    float stepProgress = this->totalSteps * q_lerp(progress, this->smoothness);
    int stepsToTake = int(round(stepProgress - this->stepsTaken));
    this->stepsTaken = stepProgress;

    Serial.println("Stepper moving to: " + String(this->angle));

    if (stepsToTake < 0)
    {
        digitalWrite(DIR, HIGH);

        for (int i = 0; i < abs(stepsToTake); i++)
        {
            digitalWrite(STEP, HIGH);
            delayMicroseconds(1000);
            digitalWrite(STEP, LOW);
            delayMicroseconds(1000);
        }
    }
    else
    {
        digitalWrite(DIR, LOW);

        for (int i = 0; i < abs(stepsToTake); i++)
        {
            digitalWrite(STEP, HIGH);
            delayMicroseconds(2000);
            digitalWrite(STEP, LOW);
            delayMicroseconds(2000);
        }
    }
}