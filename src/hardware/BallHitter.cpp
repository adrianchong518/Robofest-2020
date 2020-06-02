#include "hardware/BallHitter.h"

#include "utils.h"

hardware::BallHitter::BallHitter(hardware::Motor *const motor)
    : PID(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD, HITTER_PID_MIN,
          HITTER_PID_MAX),
      m_motor(motor) {
  m_isTargetLimitEnabled = true;
  m_targetLowerLimit = HITTER_TARGET_DEG_MIN * HITTER_ENCODER_STEP_PER_DEG;
  m_targetUpperLimit = HITTER_TARGET_DEG_MAX * HITTER_ENCODER_STEP_PER_DEG;

  m_allowedError = HITTER_DEG_ALLOWED_ERROR;
}

hardware::BallHitter::~BallHitter() {}

void hardware::BallHitter::update(const int16_t encoderLocation) {
  if (m_isTargetReached) {
    m_hasTargetBeenReached = true;
  }

  if (m_hitStage == 1 && m_isTargetReached) {
    setTarget(m_hitLowPos);
    m_hitStage = 2;
  } else if (m_hitStage == 2 && m_isTargetReached) {
    m_hitStage = 3;
  }

  int speed;
  if (m_hasTargetBeenReached && !m_isHeldWhenTargetReached) {
    speed = 0;
  } else {
    speed = calculatePID(encoderLocation);
  }

  // LOG("<Ball Hitter> Motor Speed: " + String(speed));
  m_motor->setSpeed(speed);
}

void hardware::BallHitter::stop() { m_motor->setSpeed(0); }

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
  return PID::setTarget(degree * HITTER_ENCODER_STEP_PER_DEG);
}

bool hardware::BallHitter::isTargetReached() { return m_isTargetReached; }

void hardware::BallHitter::setIsHeldWhenTargetReached(
    const bool isHeldWhenTargetReached) {
  m_isHeldWhenTargetReached = isHeldWhenTargetReached;
}

byte hardware::BallHitter::getHitStage() { return m_hitStage; }
