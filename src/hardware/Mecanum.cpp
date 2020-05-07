#include "hardware/Mecanum.h"

hardware::Mecanum::Mecanum(Motor *const wheelFL, Motor *const wheelFR,
                           Motor *const wheelBL, Motor *const wheelBR)
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

hardware::Mecanum::~Mecanum() {}

void hardware::Mecanum::update() {
  while (SERIAL_GYROSCOPE.available()) {
    JY901.CopeSerialData(SERIAL_GYROSCOPE.read());
  }

  m_rotation = (double)JY901.stcAngle.Angle[2] - m_rotationOffset;
  int rotationSpeedDiff = round(m_rotationPID.calculatePID(m_rotation));
  setRotationSpeedDiff(rotationSpeedDiff);
  setMotorsSpeeds();
}

void hardware::Mecanum::findRotationOffset() {
  double rotationalOffset = 0;

  unsigned long prevReadingTime = 0;
  int numReadings = 0;
  while (numReadings < 10) {
    while (SERIAL_GYROSCOPE.available()) {
      JY901.CopeSerialData(SERIAL_GYROSCOPE.read());
    }

    unsigned long currTime = millis();
    if (currTime - prevReadingTime >= 10) {
      prevReadingTime = currTime;

      rotationalOffset = (double)JY901.stcAngle.Angle[2] / 10.0;
      numReadings++;
    }
  }

  m_rotationOffset = rotationalOffset;
}

void hardware::Mecanum::setSpeed(const unsigned int speed) { m_speed = speed; }

void hardware::Mecanum::setDirection(const double direction) {
  m_direction = direction;

  while (m_direction > 180) {
    m_direction -= 360;
  }

  while (m_direction < -180) {
    m_direction += 360;
  }
}

double hardware::Mecanum::getRotation() { return m_rotation; }

void hardware::Mecanum::setRotationSpeedDiff(const int rotationSpeedDiff) {
  m_rotationSpeedDiff =
      constrain(rotationSpeedDiff, MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX);
}

double hardware::Mecanum::getRotationTarget() {
  return m_rotationPID.getTarget();
}

void hardware::Mecanum::setRotationTarget(const double rotationTarget) {
  m_rotationPID.setTarget(rotationTarget);
}

bool hardware::Mecanum::isRotationTargetReached() {
  return m_rotationPID.isTargetReached();
}

void hardware::Mecanum::setMotorsSpeeds() {
  double scaledSpeed = m_speed * (255 - m_rotationSpeedDiff / 2.0) / 255.0;
  double theta = m_direction - m_rotation;

  double p1 = scaledSpeed * sin(PI / 4 + theta);
  double p2 = scaledSpeed * sin(PI / 4 - theta);

  int wheelFLSpeed = round(-p1 + m_rotationSpeedDiff / 2.0);
  m_wheelFL->setSpeed(wheelFLSpeed);

  int wheelFRSpeed = round(p2 + m_rotationSpeedDiff / 2.0);
  m_wheelFR->setSpeed(wheelFRSpeed);

  int wheelBLSpeed = round(-p2 - m_rotationSpeedDiff / 2.0);
  m_wheelBL->setSpeed(wheelBLSpeed);

  int wheelBRSpeed = round(p1 - m_rotationSpeedDiff / 2.0);
  m_wheelBR->setSpeed(wheelBRSpeed);
}