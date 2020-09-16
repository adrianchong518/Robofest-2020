#include "hardware/Rail.h"

#include "hardware/interface.h"

hardware::Rail::Rail(const uint8_t pin_pulse, const uint8_t pin_dir,
                     const uint8_t pin_leftLimitSwitch,
                     const uint8_t pin_rightLimitSwitch, const double stepPerMM)
    : Stepper(pin_pulse, pin_dir, Stepper::PULSE_DIR),
      m_pin_leftLimitSwitch(pin_leftLimitSwitch),
      m_pin_rightLimitSwitch(pin_rightLimitSwitch),
      m_stepPerMM(stepPerMM) {
  pinMode(m_pin_leftLimitSwitch, INPUT_PULLUP);
  pinMode(m_pin_rightLimitSwitch, INPUT_PULLUP);

  setStepLimitEnabled(true);
  setStepLimitMM(0, 1000);
}

hardware::Rail::~Rail() {}

void hardware::Rail::update() {
  if (digitalRead(m_pin_leftLimitSwitch) == LOW) {
    m_target = m_location;
    m_stepUpperLimit = m_location;
  }

  Stepper::update();
}

void hardware::Rail::stop() {
  digitalWrite(m_pin_control1, m_inactiveState);
  digitalWrite(m_pin_control2, m_inactiveState);
}

void hardware::Rail::home(const unsigned long pulseWidth) {
  LOG_DEBUG("<Rail>\tHoming...");

  unsigned int prevPulseWidth = m_pulseWidth;
  setPulseWidth(pulseWidth);

  while (digitalRead(m_pin_rightLimitSwitch) == HIGH) {
    genPulse(-1);
  }

  setPulseWidth(prevPulseWidth);
  setHomePosition();
}

double hardware::Rail::getLocationMM() { return getLocation() / m_stepPerMM; }

void hardware::Rail::setStepLimitMM(const double stepLowerLimitMM,
                                    const double stepUpperLimitMM) {
  setStepLimit(stepLowerLimitMM * m_stepPerMM, stepUpperLimitMM * m_stepPerMM);
}

Stepper::CODES hardware::Rail::setTargetMM(const double targetMM) {
  double target = targetMM * m_stepPerMM;
  long absoluteTarget = target < 0 ? m_stepUpperLimit - target : target;
  Stepper::CODES returnCode = Stepper::setTarget(absoluteTarget);

  switch (returnCode) {
    case Stepper::NO_ERROR:
      LOG_DEBUG("<Rail>\tTarget (" + String(targetMM) + " mm) Set");
      break;

    case Stepper::ERROR_TARGET_EXCEEDS_LIMIT:
      LOG_ERROR("<Rail>\tTarget (" + String(targetMM) + " mm) Exceeds Limit");
      break;

    default:
      LOG_ERROR("<Rail>\tSet Target Unknown Error");
      break;
  }

  return returnCode;
}

Stepper::CODES hardware::Rail::setTarget(long target) {
  long absoluteTarget = target < 0 ? m_stepUpperLimit - target : target;
  Stepper::CODES returnCode = Stepper::setTarget(absoluteTarget);

  switch (returnCode) {
    case Stepper::NO_ERROR:
      LOG_DEBUG("<Rail>\tTarget (" + String(target) + " steps) Set");
      break;

    case Stepper::ERROR_TARGET_EXCEEDS_LIMIT:
      LOG_ERROR("<Rail>\tTarget (" + String(target) + " steps) Exceeds Limit");
      break;

    default:
      LOG_ERROR("<Rail>\tSet Target Unknown Error");
      break;
  }

  return returnCode;
}
