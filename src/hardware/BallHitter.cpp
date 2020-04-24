#include "hardware/BallHitter.h"

hardware::BallHitter::BallHitter(hardware::Motor *const motor)
    : m_motor(motor),
      m_pid(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD, HITTER_PID_MIN,
            HITTER_PID_MAX) {
  m_pid.setTargetLimitEnabled(true);
  m_pid.setTargetLimit(HITTER_TARGET_DEG_MIN * HITTER_ENCODER_STEP_PER_DEG,
                       HITTER_TARGET_DEG_MAX * HITTER_ENCODER_STEP_PER_DEG);
  m_pid.setAllowedError(HITTER_DEG_ALLOWED_ERROR * HITTER_ENCODER_STEP_PER_DEG);
}

hardware::BallHitter::~BallHitter() {}

void hardware::BallHitter::update(const uint16_t encoderLocation) {
  if (m_pid.isTargetReached()) {
    m_hasTargetBeenReached = true;
  }

  if (m_hitStage == 1 && m_pid.isTargetReached()) {
    setTarget(m_hitLowPos);
    m_hitStage = 2;
  } else if (m_hitStage == 2 && m_pid.isTargetReached()) {
    m_hitStage = 3;
  }

  int speed;
  if (m_hasTargetBeenReached && !m_isHeldWhenTargetReached) {
    speed = 0;
  } else {
    speed = m_pid.calculatePID(encoderLocation);
  }

  m_motor->setSpeed(speed);
}

PID::CODES hardware::BallHitter::hit(const double highPos,
                                     const double lowPos) {
  if ((highPos < HITTER_TARGET_DEG_MIN || highPos > HITTER_TARGET_DEG_MAX) ||
      (highPos < HITTER_TARGET_DEG_MIN || highPos > HITTER_TARGET_DEG_MAX)) {
    return PID::ERROR_TARGET_EXCEEDS_LIMIT;
  }

  m_hitStage = 1;
  m_hitHighPos = highPos;
  m_hitLowPos = lowPos;

  setTarget(m_hitHighPos * HITTER_ENCODER_STEP_PER_DEG);
  return PID::NO_ERROR;
}

PID::CODES hardware::BallHitter::setTarget(const double degree) {
  m_hasTargetBeenReached = false;
  return m_pid.setTarget(degree * HITTER_ENCODER_STEP_PER_DEG);
}

bool hardware::BallHitter::isTargetReached() { return m_pid.isTargetReached(); }

void hardware::BallHitter::setIsHeldWhenTargetReached(
    const bool isHeldWhenTargetReached) {
  m_isHeldWhenTargetReached = isHeldWhenTargetReached;
}

byte hardware::BallHitter::getHitStage() { return m_hitStage; }
