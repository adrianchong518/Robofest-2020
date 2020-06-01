#include "hardware/Rail.h"

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

void hardware::Rail::stop() {
  digitalWrite(m_pin_control1, m_inactiveState);
  digitalWrite(m_pin_control2, m_inactiveState);
}

void hardware::Rail::home(const unsigned long pulseWidth) {
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
  return setTarget(targetMM * m_stepPerMM);
}
