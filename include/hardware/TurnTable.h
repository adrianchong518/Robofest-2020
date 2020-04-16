#ifndef HARDWARE_TURN_TABLE_H
#define HARDWARE_TURN_TABLE_H

#include <Arduino.h>
#include <Stepper.h>

namespace hardware {

class TurnTable : public Stepper {
 private:
  uint8_t m_pin_homeSensor;

  int m_sensorThreshold;
  double m_stepPerDeg;

 public:
  TurnTable(const uint8_t pin_cw, const uint8_t pin_ccw,
            const uint8_t pin_homeSensor, const int sensorThreshold,
            const double stepPerDeg);
  ~TurnTable();

  void home();

  double getLocationDeg();

  void setStepLimitDeg(const double stepLowerLimitDeg,
                       const double stepUpperLimitDeg);

  CODES setTargetDeg(const double targetDeg);
};

}  // namespace hardware

#endif  // HARDWARE_TURN_TABLE_H
