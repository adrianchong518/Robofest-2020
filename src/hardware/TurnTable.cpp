#include "hardware/TurnTable.h"

#include "hardware/interface.h"

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
  LOG_DEBUG("<Turn Table> Homing...");

  unsigned int prevPulseWidth = m_pulseWidth;
  setPulseWidth(pulseWidth);

  while (analogRead(m_pin_homeSensor) <= m_sensorThreshold) {
    genPulse(1);
  }

  setHomePosition();
  setPulseWidth(prevPulseWidth);

  setTargetDeg(30);
  while (!isTargetReached()) {
    update();
  }
}

double hardware::TurnTable::getLocationDeg() {
  return getLocation() / m_stepPerDeg;
}

void hardware::TurnTable::setStepLimitDeg(const double stepLowerLimitDeg,
                                          const double stepUpperLimitDeg) {
  setStepLimit(stepLowerLimitDeg * m_stepPerDeg,
               stepUpperLimitDeg * m_stepPerDeg);
}

Stepper::CODES hardware::TurnTable::setTargetDeg(const double targetDeg) {
  Stepper::CODES returnCode = Stepper::setTarget(targetDeg * m_stepPerDeg);

  switch (returnCode) {
    case hardware::TurnTable::NO_ERROR:
      LOG_DEBUG("<Turn Table> Target (" + String(targetDeg) + " deg) Set");
      break;

    case hardware::TurnTable::ERROR_TARGET_EXCEEDS_LIMIT:
      LOG_ERROR("<Turn Table> Target (" + String(targetDeg) +
                " deg) Exceeds Limit");
      break;

    default:
      LOG_ERROR("<Turn Table> Unknown Error");
      break;
  }

  return returnCode;
}

Stepper::CODES hardware::TurnTable::setTarget(long target) {
  Stepper::CODES returnCode = Stepper::setTarget(target);

  switch (returnCode) {
    case hardware::TurnTable::NO_ERROR:
      LOG_DEBUG("<Turn Table> Target (" + String(target) + " steps) Set");
      break;

    case hardware::TurnTable::ERROR_TARGET_EXCEEDS_LIMIT:
      LOG_ERROR("<Turn Table> Target (" + String(target) +
                " steps) Exceeds Limit");
      break;

    default:
      LOG_ERROR("<Turn Table> Unknown Error");
      break;
  }

  return returnCode;
}