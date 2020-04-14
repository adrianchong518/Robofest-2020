#include "hardware/TurnTable.h"

TurnTable::TurnTable(const uint8_t pin_cw, const uint8_t pin_ccw,
                     const uint8_t pin_homeSensor, const int sensorThreshold,
                     const double stepPerDeg)
    : Stepper(pin_cw, pin_ccw, CW_CCW_PULSE), m_pin_homeSensor(pin_homeSensor),
      m_sensorThreshold(sensorThreshold), m_stepPerDeg(stepPerDeg) {
  pinMode(m_pin_homeSensor, INPUT);
}

TurnTable::~TurnTable() {}

void TurnTable::home() {
  unsigned int prevPulseWidth = m_pulseWidth;
  setPulseWidth(m_maxPulseWidth);

  while (analogRead(m_pin_homeSensor) <= m_sensorThreshold) {
    genPulse(1);
  }

  setHomePosition();
  setPulseWidth(prevPulseWidth);
}

double TurnTable::getLocationDeg() { return getLocation() / m_stepPerDeg; }

void TurnTable::setStepLimitDeg(const double stepLowerLimitDeg,
                                const double stepUpperLimitDeg) {
  setStepLimit(stepLowerLimitDeg * m_stepPerDeg,
               stepUpperLimitDeg * m_stepPerDeg);
}

TurnTable::CODES TurnTable::setTargetDeg(const double targetDeg) {
  return setTarget(targetDeg * m_stepPerDeg);
}
