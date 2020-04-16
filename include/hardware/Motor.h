#ifndef HARDWARE_MOTOR_H
#define HARDWARE_MOTOR_H

#include <Arduino.h>

namespace hardware {

class Motor {
 private:
  uint8_t m_pin_inA;
  uint8_t m_pin_inB;
  uint8_t m_pin_PWM;

 public:
  Motor(const uint8_t pin_inA, const uint8_t pin_inB, const uint8_t pin_PWM);
  ~Motor();

  void setSpeed(const int speed);
};

}  // namespace hardware

#endif
