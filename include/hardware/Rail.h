#ifndef HARDWARE_RAIL_H
#define HARDWARE_RAIL_H

#include <Arduino.h>
#include <Stepper.h>

namespace hardware {

class Rail : public Stepper {
 private:
  uint8_t m_pin_leftLimitSwitch;
  uint8_t m_pin_rightLimitSwitch;

  double m_stepPerMM;

  long m_relativeTarget;

 public:
  Rail(const uint8_t pin_pulse, const uint8_t pin_dir,
       const uint8_t pin_leftLimitSwitch, const uint8_t pin_rightLimitSwitch,
       const double stepPerMM);
  ~Rail();

  void update();

  void stop();

  void home(const unsigned long pulseWidth);

  double getLocationMM();

  void setStepLimitMM(const double stepLowerLimitMM,
                      const double stepUpperLimitMM);

  CODES setTargetMM(const double targetMM);
  CODES setTarget(long target);
};

}  // namespace hardware

#endif  // HARDWARE_RAIL_H
