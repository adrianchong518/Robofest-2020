#include "hardware/servos/CustomServo.h"

#include "utils.h"

hardware::servos::CustomServo::CustomServo(uint8_t pin,
                                           unsigned long msPerDegree)
    : Servo(), m_msPerDegree(msPerDegree) {
  attach(pin);
  write(m_pos);
}

void hardware::servos::CustomServo::update() {
  if (!isTargetReached()) {
    if (millis() - m_prevUpdateTime >= m_msPerDegree) {
      m_pos += constrain(m_target - m_pos, -1, 1);
      write(m_pos);
      LOG("Servo: " + String(m_pos) + " | " + String(m_target));
      m_prevUpdateTime += m_msPerDegree;
    }
  }
}

int hardware::servos::CustomServo::getTarget() { return m_target; }

void hardware::servos::CustomServo::setTarget(int target) {
  m_target = constrain(target, 0, 180);
  m_prevUpdateTime = millis();
}

int hardware::servos::CustomServo::getPosition() { return m_pos; }

bool hardware::servos::CustomServo::isTargetReached() {
  return m_target == m_pos;
}
