#include "TurnTable.h"

TurnTable::TurnTable(const uint8_t pin_cw, const uint8_t pin_ccw,
                     const uint8_t pin_homeSensor, const int sensorThreshold,
                     const double degPerStep)
    : Stepper(pin_cw, pin_ccw, CW_CCW_PULSE),
      m_pin_homeSensor(pin_homeSensor),
      m_sensorThreshold(sensorThreshold),
      m_degPerStep(degPerStep) {}

TurnTable::~TurnTable() {}

void TurnTable::home() {
  unsigned int prevPulseWidth = m_pulseWidth;
  setPulseWidth(m_maxPulseWidth);

  while (analogRead(m_pin_homeSensor) <= m_sensorThreshold) {
    genPulse(1)
  }

  setHomePosition();
  setPulseWidth(prevPulseWidth);
}

double TurnTable::getLocationDeg() { return getLocation() * m_degPerStep; }

void TurnTable::setStepLimitDeg(const double stepLowerLimitDeg,
                                const double stepUpperLimitDeg) {
  setStepLimit(stepLowerLimitDeg / m_degPerStep,
               stepUpperLimitDeg / m_degPerStep);
}

TurnTable::CODES TurnTable::setTargetDeg(const double targetDeg) {
  return setTarget(targetDeg / m_degPerStep);
}