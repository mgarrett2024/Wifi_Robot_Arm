#include "helper.h"

// Helper Functions

float q_lerp(float _value, float _ease) {
  return pow(_value, _ease) / (pow(_value, _ease) + pow(1-_value,_ease));
}

float lerp(float _start, float _end, float _value) {
  return (_value * _end) + ((1 - _value) * _start);
}

float normalize(float _value, float _minimum, float _maximum) {
  return (_value - _minimum) / (_maximum - _minimum);
}

float clamp(float _value, float _minimum, float _maximum) {
  return max(_minimum, min(_maximum, _value));
}

float map_to_normal(float _value, float _minimum, float _maximum, float _normal) {
  return ((_value - _minimum) / (_maximum - _minimum)) * _normal;
}
