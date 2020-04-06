#ifndef MECANUM_H
#define MECANUM_H

#include <PID.h>

#include "Motor.h"
#include "Constants.h"

class Mecanum {
 private:
  Motor* const m_wheelFL;
  Motor* const m_wheelFR;
  Motor* const m_wheelBL;
  Motor* const m_wheelBR;

  PID m_rotationPID;

  unsigned int m_speed;
  double m_direction;
  double m_rotation;
  int m_rotationalSpeedDiff;

 public:
  Mecanum(Motor* const wheelFL, Motor* const wheelFR, Motor* const wheelBL,
          Motor* const wheelBR);
  ~Mecanum();

  void update(const double rotation);

  void setSpeed(const unsigned int speed);

  void setDirection(const double direction);

  void setRotationSpeedDiff(const int rotationSpeedDiff);

  double getRotationTarget();
  void setRotationTarget(const double rotationTarget);

  bool isRotationTargetReached();

  void setMotorsSpeeds();
};

#endif  // MECANUM_H