#ifndef HARDWARE_SERVO_H
#define HARDWARE_SERVO_H

#include <Arduino.h>
#include <Servo.h>

namespace hardware {
namespace servos {

class CustomServo : private Servo {
 private:
  int m_target = 90;
  int m_pos = 90;

  unsigned long m_msPerDegree;
  unsigned long m_prevUpdateTime;

 public:
  CustomServo(uint8_t pin, unsigned long msPerDegree);

  void update();

  int getTarget();
  void setTarget(int target);

  int getPosition();
  bool isTargetReached();
};

}  // namespace servos
}  // namespace hardware

#endif  // HARDWARE_SERVO_H
