#include "hardware/TurnTable.h"

hardware::TurnTable::TurnTable(const uint8_t pin_cw, const uint8_t pin_ccw,
                               const uint8_t pin_homeSensor,
                               const int sensorThreshold,
                               const double stepPerDeg)
    : Stepper(pin_cw, pin_ccw, CW_CCW_PULSE, HIGH),
      m_pin_homeSensor(pin_homeSensor),
      m_sensorThreshold(sensorThreshold),
      m_stepPerDeg(stepPerDeg) {
  pinMode(m_pin_homeSensor, INPUT);
}

hardware::TurnTable::~TurnTable() {}

void hardware::TurnTable::stop() {
  digitalWrite(m_pin_control1, m_inactiveState);
  digitalWrite(m_pin_control2, m_inactiveState);
}

void hardware::TurnTable::home(const unsigned long pulseWidth) {
  unsigned int prevPulseWidth = m_pulseWidth;
  setPulseWidth(pulseWidth);

  while (analogRead(m_pin_homeSensor) <= m_sensorThreshold) {
    genPulse(1);
  }

  setHomePosition();
  setPulseWidth(prevPulseWidth);
}

double hardware::TurnTable::getLocationDeg() {
  return getLocation() / m_stepPerDeg;
}

void hardware::TurnTable::setStepLimitDeg(const double stepLowerLimitDeg,
                                          const double stepUpperLimitDeg) {
  setStepLimit(stepLowerLimitDeg * m_stepPerDeg,
               stepUpperLimitDeg * m_stepPerDeg);
}

hardware::TurnTable::CODES hardware::TurnTable::setTargetDeg(
    const double targetDeg) {
  return setTarget(targetDeg * m_stepPerDeg);
}
