#include "hardware/BallHitter.h"

#include "utils.h"

hardware::BallHitter::BallHitter(hardware::Motor *const motor)
    : PID(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD, HITTER_PID_MIN,
          HITTER_PID_MAX),
      m_motor(motor) {
  m_isTargetLimitEnabled = true;
  m_targetLowerLimit = HITTER_TARGET_DEG_MIN * HITTER_ENCODER_STEP_PER_DEG;
  m_targetUpperLimit = HITTER_TARGET_DEG_MAX * HITTER_ENCODER_STEP_PER_DEG;

  m_allowedError = HITTER_DEG_ALLOWED_ERROR * HITTER_ENCODER_STEP_PER_DEG;
}

hardware::BallHitter::~BallHitter() {}

void hardware::BallHitter::update(const int16_t encoderLocation) {
  if (m_isTargetReached) {
    m_hasTargetBeenReached = true;
  }

  int speed;
  if (m_hasTargetBeenReached && !m_isHeldWhenTargetReached) {
    speed = 0;
  } else if (m_hitStage == 2) {
    speed = -255;
  } else {
    speed = calculatePID(encoderLocation);
  }

  if (m_hitStage == 1 && m_isTargetReached) {
    LOG("<Ball Hitter> High Position Reached");
    speed = -255;
    m_hitStage = 2;
  } else if (m_hitStage == 2 && encoderLocation <= m_hitLowPos) {
    LOG("<Ball Hitter> Low Position Reached");
    setTarget(0);
    m_hitStage = 0;
  }

  // LOG("<Ball Hitter> Motor Speed: " + String(speed));
  m_motor->setSpeed(speed);
}

void hardware::BallHitter::stop() { m_motor->setSpeed(0); }

PID::CODES hardware::BallHitter::hit(const double highPos,
                                     const double lowPos) {
  if ((highPos < HITTER_TARGET_DEG_MIN || highPos > HITTER_TARGET_DEG_MAX) ||
      (lowPos < HITTER_TARGET_DEG_MIN || lowPos > HITTER_TARGET_DEG_MAX)) {
    return PID::ERROR_TARGET_EXCEEDS_LIMIT;
  }

  m_hitStage = 1;
  m_hitHighPos = highPos * HITTER_ENCODER_STEP_PER_DEG;
  m_hitLowPos = lowPos * HITTER_ENCODER_STEP_PER_DEG;

  m_hasTargetBeenReached = false;
  PID::setTarget(m_hitHighPos);
  return PID::NO_ERROR;
}

PID::CODES hardware::BallHitter::setTarget(const double degree) {
  m_hasTargetBeenReached = false;
  m_hitStage = 0;
  return PID::setTarget(degree * HITTER_ENCODER_STEP_PER_DEG);
}

void hardware::BallHitter::setIsHeldWhenTargetReached(
    const bool isHeldWhenTargetReached) {
  m_isHeldWhenTargetReached = isHeldWhenTargetReached;
}

byte hardware::BallHitter::getHitStage() { return m_hitStage; }
