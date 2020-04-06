#include "Mecanum.h"

Mecanum::Mecanum(Motor* const wheelFL, Motor* const wheelFR,
                 Motor* const wheelBL, Motor* const wheelBR)
    : m_wheelFL(wheelFL),
      m_wheelFR(wheelFR),
      m_wheelBL(wheelBL),
      m_wheelBR(wheelBR),
      m_rotationPID(MECANUM_ROT_PID_KP, MECANUM_ROT_PID_KI, MECANUM_ROT_PID_KD,
                    MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX) {
  m_rotationPID.setTargetLimitEnabled(true);
  m_rotationPID.setTargetLimit(-180, 180);

  SERIAL_GYROSCOPE.begin(SERIAL_GYROSCOPE_BAUDRATE);
}

Mecanum::~Mecanum() {}

void Mecanum::update() {
  while (SERIAL_GYROSCOPE.available) {
    JY901.CopeSerialData(SERIAL_GYROSCOPE.read());
  }

  m_rotation = (double)JY901.stcAngle.Angle[2];
  int rotationSpeedDiff = round(m_rotationPID.calculatePID(m_rotation));
  setRotationSpeedDiff(rotationSpeedDiff);
  setMotorsSpeeds();
}

void Mecanum::setSpeed(const unsigned int speed) { m_speed = speed; }

void Mecanum::setDirection(const double direction) {
  m_direction = direction;

  while (m_direction > 180) {
    m_direction -= 360;
  }

  while (m_direction < -180) {
    m_direction += 360;
  }
}

double Mecanum::getRotation() { return m_rotation; }

void Mecanum::setRotationSpeedDiff(const int rotationSpeedDiff) {
  m_rotationalSpeedDiff =
      constrain(rotationSpeedDiff, MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX);
}

double Mecanum::getRotationTarget() { return m_rotationPID.getTarget(); }

void Mecanum::setRotationTarget(const double rotationTarget) {
  m_rotationPID.setTarget(rotationTarget);
}

bool Mecanum::isRotationTargetReached() {
  return m_rotationPID.isTargetReached();
}

void Mecanum::setMotorsSpeeds() {
  // TODO Calculate motor speeds
  int wheelFLSpeed;
  m_wheelFL->setSpeed(wheelFLSpeed);

  int wheelFRSpeed;
  m_wheelFR->setSpeed(wheelFRSpeed);

  int wheelBLSpeed;
  m_wheelBL->setSpeed(wheelBLSpeed);

  int wheelBRSpeed;
  m_wheelBR->setSpeed(wheelBRSpeed);
}