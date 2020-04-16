#include "hardware/Motor.h"

hardware::Motor::Motor(const uint8_t pin_inA, const uint8_t pin_inB,
                       const uint8_t pin_PWM)
    : m_pin_inA(pin_inA), m_pin_inB(pin_inB), m_pin_PWM(pin_PWM) {
  pinMode(m_pin_inA, OUTPUT);
  pinMode(m_pin_inB, OUTPUT);
  pinMode(m_pin_PWM, OUTPUT);

  setSpeed(0);
}

hardware::Motor::~Motor() {}

void hardware::Motor::setSpeed(const int speed) {
  if (speed > 0) {
    digitalWrite(m_pin_inA, HIGH);
    digitalWrite(m_pin_inB, LOW);
  } else if (speed < 0) {
    digitalWrite(m_pin_inA, LOW);
    digitalWrite(m_pin_inB, HIGH);
  } else {
    digitalWrite(m_pin_inA, HIGH);
    digitalWrite(m_pin_inB, HIGH);
  }

  analogWrite(m_pin_PWM, constrain(abs(speed), 0, 255));
}
