#ifndef MECANUM_H
#define MECANUM_H

#include <JY901.h>
#include <PID.h>

#include "Constants.h"
#include "hardware/Motor.h"

class Mecanum {
private:
  Motor *const m_wheelFL;
  Motor *const m_wheelFR;
  Motor *const m_wheelBL;
  Motor *const m_wheelBR;

  PID m_rotationPID;

  unsigned int m_speed;
  double m_direction;
  double m_rotation;
  int m_rotationSpeedDiff;
  double m_rotationOffset;

public:
  Mecanum(Motor *const wheelFL, Motor *const wheelFR, Motor *const wheelBL,
          Motor *const wheelBR);
  ~Mecanum();

  void update();

  void findRotationOffset();

  void setSpeed(const unsigned int speed);

  void setDirection(const double direction);

  double getRotation();

  void setRotationSpeedDiff(const int rotationSpeedDiff);

  double getRotationTarget();
  void setRotationTarget(const double rotationTarget);

  bool isRotationTargetReached();

  void setMotorsSpeeds();
};

#endif // MECANUM_H
