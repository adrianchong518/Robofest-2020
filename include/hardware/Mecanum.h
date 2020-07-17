#ifndef HARDWARE_MECANUM_H
#define HARDWARE_MECANUM_H

#include <JY901.h>
#include <PID.h>

#include "constants.h"
#include "hardware/Motor.h"

namespace hardware {

class Mecanum : public PID {
 public:
  bool isEnabled = true;
  bool isGyroEnabled = true;

 private:
  Motor *const m_wheelFL;
  Motor *const m_wheelFR;
  Motor *const m_wheelBL;
  Motor *const m_wheelBR;

  unsigned int m_speed;
  double m_direction;
  double m_rotation;
  int m_rotationSpeedDiff;
  double m_rotationOffset;

  int m_wheelFLSpeed = 0;
  int m_wheelFRSpeed = 0;
  int m_wheelBLSpeed = 0;
  int m_wheelBRSpeed = 0;

 public:
  Mecanum(Motor *const wheelFL, Motor *const wheelFR, Motor *const wheelBL,
          Motor *const wheelBR);
  ~Mecanum();

  void update();

  void stop();

  void findRotationOffset();

  void setSpeed(const unsigned int speed);

  void setDirection(const double direction);

  double getRotation();

  void setRotationSpeedDiff(const int rotationSpeedDiff);

  void setTarget(double rotationTarget);

  void getMotorsSpeeds(int &wheelFLSpeed, int &wheelFRSpeed, int &wheelBLSpeed,
                       int &wheelBRSpeed);
  void setMotorsSpeeds();

  void setIsGyroEnabled(bool isGyroEnabled);
};

}  // namespace hardware

#endif  // HARDWARE_MECANUM_H
