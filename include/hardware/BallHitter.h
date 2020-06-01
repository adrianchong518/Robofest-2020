#ifndef HARDWARE_BALL_HITTER_H
#define HARDWARE_BALL_HITTER_H

#include <Arduino.h>
#include <PID.h>

#include "constants.h"
#include "hardware/Motor.h"

namespace hardware {

class BallHitter {
 private:
  Motor *const m_motor;
  PID m_pid;

  bool m_isHeldWhenTargetReached = true;
  bool m_hasTargetBeenReached = true;

  byte m_hitStage = 0;
  double m_hitHighPos;
  double m_hitLowPos;

 public:
  BallHitter(Motor *const motor);
  ~BallHitter();

  void update(const uint16_t encoderLocation);

  void stop();

  PID::CODES hit(const double highPos, const double lowPos);

  PID::CODES setTarget(const double degree);

  bool isTargetReached();

  void setIsHeldWhenTargetReached(const bool isHeldWhenTargetReached);

  byte getHitStage();
};

}  // namespace hardware

#endif  // HARDWARE_BALL_HITTER_H
