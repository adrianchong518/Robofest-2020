#include "hardware/BallHitter.h"

#include "hardware/interface.h"

hardware::BallHitter::BallHitter(hardware::Motor *const motor)
    : PID(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD, HITTER_PID_MIN,
          HITTER_PID_MAX),
      m_motor(motor) {
  m_isTargetLimitEnabled = true;
  m_targetLowerLimit = HITTER_TARGET_DEG_MIN * HITTER_ENCODER_STEP_PER_DEG;
  m_targetUpperLimit = HITTER_TARGET_DEG_MAX * HITTER_ENCODER_STEP_PER_DEG;

  m_allowedError = HITTER_DEG_ALLOWED_ERROR * HITTER_ENCODER_STEP_PER_DEG;

  setTarget(HITTER_REST_POS);
}

hardware::BallHitter::~BallHitter() {}

void hardware::BallHitter::update(const int16_t encoderLocation) {
  if (m_isTargetReached) {
    m_hasTargetBeenReached = true;
  }

  int speed;
  if (m_hasTargetBeenReached && !m_isHeldWhenTargetReached) {
    speed = 0;
  } else if (m_hitStage == 1) {
    speed = HITTER_PULL_SPEED;
  } else if (m_hitStage == 2) {
    speed = HITTER_HIT_SPEED;
  } else {
    speed = calculatePID(encoderLocation);
  }

  if (m_hitStage == 1 && encoderLocation >= m_hitHighPos) {
    LOG_DEBUG("<Ball Hitter>\tHigh Position Reached");
    speed = HITTER_HIT_SPEED;
    m_hitStage = 2;
  } else if (m_hitStage == 2 && encoderLocation <= m_hitLowPos) {
    LOG_DEBUG("<Ball Hitter>\tLow Position Reached");
    setTarget(HITTER_REST_POS);
    m_hitStage = 3;
  } else if (m_hitStage == 3 && isTargetReached()) {
    m_hitStage = 0;
  }

  m_motor->setSpeed(speed);
}

void hardware::BallHitter::stop() { m_motor->setSpeed(0); }

PID::CODES hardware::BallHitter::hit(const double highPos,
                                     const double lowPos) {
  if ((highPos < HITTER_TARGET_DEG_MIN || highPos > HITTER_TARGET_DEG_MAX) ||
      (lowPos < HITTER_TARGET_DEG_MIN || lowPos > HITTER_TARGET_DEG_MAX)) {
    LOG_ERROR("<Ball Hitter>\tHit Targets (" + String(highPos) + "," +
              String(lowPos) + ") Exceeds Limit");
    return PID::ERROR_TARGET_EXCEEDS_LIMIT;
  }

  m_hitStage = 1;
  m_hitHighPos = highPos * HITTER_ENCODER_STEP_PER_DEG;
  m_hitLowPos = lowPos * HITTER_ENCODER_STEP_PER_DEG;

  LOG_DEBUG("<Ball Hitter>\tHit Targets (" + String(highPos) + "," +
            String(lowPos) + ") Set");
  return PID::NO_ERROR;
}

PID::CODES hardware::BallHitter::setTarget(const double degree) {
  m_hasTargetBeenReached = false;
  m_hitStage = 0;
  PID::CODES returnCode = PID::setTarget(degree * HITTER_ENCODER_STEP_PER_DEG);

  switch (returnCode) {
    case PID::NO_ERROR:
      LOG_DEBUG("<Ball Hitter>\tTarget (" + String(degree) + ") Set");
      break;

    case PID::ERROR_TARGET_EXCEEDS_LIMIT:
      LOG_ERROR("<Ball Hitter>\tTarget (" + String(degree) + ") Exceeds Limit");
      break;

    default:
      LOG_ERROR("<Ball Hitter>\tSet Target Unknown Error");
      break;
  }

  return returnCode;
}

void hardware::BallHitter::setIsHeldWhenTargetReached(
    const bool isHeldWhenTargetReached) {
  m_isHeldWhenTargetReached = isHeldWhenTargetReached;
}

byte hardware::BallHitter::getHitStage() { return m_hitStage; }
