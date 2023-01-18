# Wifi Robot Arm

## Background
Robot arms have always interested me and before my class I had been wanting to learn embedded systems to create my own projects. I would watch Youtube videos
about custom LED light set ups and home automation projects people had done. I decided to try to create my own robot arm for my final project after seeing 
a couple videos on Youtube. I decided on just simple stepper and servo motors for this project because I had a deadline to meet. I hope to create a better arm in
the future that utilizes some type of gearbox like a cycloidal or harmonic drive and uses all stepper or dc motors.

## About the arm
It uses 1 Nema 17 stepper motor for the base and 4 servo motors for the other joints. In order to allow the base to move in 360 degrees, all components are
housed in the base and power is delivered using a slip ring. 12v is delivered directly to the Nema 17 and into a pair of buck converters which drop the voltage
to 8.4v and 5v to control both the servos and ESP32. The motors are controlled by the ESP32 module and C++. Commands are sent the the arm by a web interface with
a series of sliders and a send button. The angles are then given to each respective motor and the motors move to those angles. The servo motors move fairly quickly
so I slowed them down by smoothly interpolating between the angle they are currently at and their target angle. They start slow and build up speed and then slow
down again as they near their target position.
