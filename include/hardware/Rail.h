#ifndef TURN_TABLE_H
#define TURN_TABLE_H

#include <Arduino.h>

#include <Stepper.h>

class Rail : public Stepper {
private:
  uint8_t m_pin_leftLimitSwitch;
  uint8_t m_pin_rightLimitSwitch;

  double m_stepPerMM;

public:
  Rail(const uint8_t pin_pulse, const uint8_t pin_dir,
       const uint8_t pin_leftLimitSwitch, const uint8_t pin_rightLimitSwitch,
       const double stepPerMM);
  ~Rail();

  void update();

  void home();

  double getLocationMM();

  void setStepLimitMM(const double stepLowerLimitMM,
                      const double stepUpperLimitMM);

  CODES setTargetMM(const double targetMM);
};

#endif // TURN_TABLE_H
