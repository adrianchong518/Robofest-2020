#ifndef STEPPER_DEGREE_H
#define STEPPER_DEGREE_H

#include <Arduino.h>

#include <Stepper.h>

class TurnTable : public Stepper {
 private:
  uint8_t m_pin_homeSensor;

  int m_sensorThreshold;
  double m_degPerStep;

 public:
  TurnTable(const uint8_t pin_cw, const uint8_t pin_ccw,
            const uint8_t pin_homeSensor, const int sensorThreshold,
            const double degPerStep);
  ~TurnTable();

  void home();

  double getLocationDeg();

  void setStepLimitDeg(const double stepLowerLimitDeg,
                       const double stepUpperLimitDeg);

  CODES setTargetDeg(const double targetDeg);
};

#endif  // STEPPER_DEGREE_H