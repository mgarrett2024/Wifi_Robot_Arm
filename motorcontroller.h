#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include "stepper.h"
#include "servomotor.h"
#include <list>
#include <vector>

class MotorController
{
private:
    std::vector<ServoMotor> servos;
    Stepper stepper;
    float speed;
    float frametime;
public:
    //Constructor
    MotorController(float speed, float frametime);
    MotorController() = default;

    void setup_servos(std::list<std::list<int>> servo_list);
    void setup_stepper(std::list<int> stepper_params);

    void move_motors_smooth(std::list<int> angles);
    void move_motors(std::list<int> angles);
};

#endif