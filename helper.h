#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>

float q_lerp(float _value, float _ease);

float lerp(float _start, float _end, float _value);

float normalize(float _value, float _minimum, float _maximum);

float clamp(float _value, float _minimum, float _maximum);

#endif