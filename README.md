# Wifi Robot Arm


# Background
Robot arms have always interested me and before my class I had been wanting to learn embedded systems to create my own projects. I would watch Youtube videos
about custom LED light set ups and home automation projects people had done. I decided to try to create my own robot arm for my final project after seeing 
a couple videos on Youtube. I decided on just simple stepper and servo motors for this project because I had a deadline to meet. I hope to create a better arm in
the future that utilizes some type of gearbox like a cycloidal or harmonic drive and uses all stepper or dc motors.


# About the arm

## Part List
- 1 Nema 17 Stepper
- 4 Servos of varying torques
- Claw Addon
- Slip ring
- 3D printed base pieces
- Aluminum square tubing


## How it works
12v is delivered to the arm through a slip ring in the base. All motors and controlling components are housed inside the arm. This allows the arm to rotate 360 degrees
continuously. The voltage is lowered to 8.4v and 5v to allow power to also be given to the ESP32 and servo motors in other joints. Commands are sent to the arm over 
WiFi from creating a web server with the ESP32. The website consists of a series of sliders, one for each joint, and a send button to send the command. The motors 
then are given the angles and they move to that position.

## Problems I Faced

### Motors moving too quickly and causing arm to be thrown around
> I fixed this problem by smoothing the motion of the arms over a sigmoid function so the motors ease into starting and stopping.


### Servo motors wouldn't move even though they were set up correctly
> I used an ESP32 Servo library for moving the motors and when moving the motors from the motorcontroller they wouldn't move even though they should've. I fixed
> this problem by "reattaching", as the library called it, the motors everytime they were given a new command


### Base stepper motor is not stronng enough to move arm when extended
> I haven't fixed this yet but I would either get a stronger stepper motor or use a type off gearbox in the base to increase torque


## Things I would change in the future
I would use all stepper motors in the future with some sort of gearbox, such as a cycloidal or harmonic drive, instead of servo motors. 


# Renders of Arm
![Full Robot Arm](https://github.com/mgarrett2024/Wifi_Robot_Arm/blob/master/Robot_Base_Full.jpg)

![Base Robot Arm](https://github.com/mgarrett2024/Wifi_Robot_Arm/blob/master/Robot_Base_Bottom.jpg)
