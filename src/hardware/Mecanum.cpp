#include "hardware/Mecanum.h"

#include "utils.h"

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
  if (m_isGyroEnabled) {
    while (SERIAL_GYROSCOPE.available()) {
      JY901.CopeSerialData(SERIAL_GYROSCOPE.read());
    }

    m_rotation =
        (double)-JY901.stcAngle.Angle[2] / 32768 * PI - m_rotationOffset;
    m_rotationSpeedDiff = round(m_rotationPID.calculatePID(m_rotation));
  } else {
    m_rotation = 0;
  }
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
    if (currTime - prevReadingTime >= 100) {
      prevReadingTime = currTime;

      rotationalOffset += (double)-JY901.stcAngle.Angle[2] / 10.0;
      numReadings++;
    }
  }

  m_rotationOffset = rotationalOffset / 32768 * PI;
  LOG("<Mecanum> Rotational Offset Set: " + String(degrees(m_rotationOffset)));
}

void hardware::Mecanum::setSpeed(const unsigned int speed) { m_speed = speed; }

void hardware::Mecanum::setDirection(const double direction) {
  m_direction = direction;

  while (m_direction > PI) {
    m_direction -= 2 * PI;
  }

  while (m_direction < -PI) {
    m_direction += 2 * PI;
  }
}

double hardware::Mecanum::getRotation() { return degrees(m_rotation); }

void hardware::Mecanum::setRotationSpeedDiff(const int rotationSpeedDiff) {
  m_rotationSpeedDiff =
      constrain(rotationSpeedDiff, MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX);
}

double hardware::Mecanum::getRotationTarget() {
  return m_rotationPID.getTarget();
}

void hardware::Mecanum::setRotationTarget(double rotationTarget) {
  while (rotationTarget > PI) {
    rotationTarget -= 2 * PI;
  }

  while (rotationTarget < -PI) {
    rotationTarget += 2 * PI;
  }

  m_rotationPID.setTarget(rotationTarget);
}

bool hardware::Mecanum::isRotationTargetReached() {
  return m_rotationPID.isTargetReached();
}

void hardware::Mecanum::getMotorsSpeeds(int &wheelFLSpeed, int &wheelFRSpeed,
                                        int &wheelBLSpeed, int &wheelBRSpeed) {
  wheelFLSpeed = m_wheelFLSpeed;
  wheelFRSpeed = m_wheelFRSpeed;
  wheelBLSpeed = m_wheelBLSpeed;
  wheelBRSpeed = m_wheelBRSpeed;
}

void hardware::Mecanum::setMotorsSpeeds() {
  double scaledSpeed = m_speed * (255 - m_rotationSpeedDiff / 2.0) / 255.0;
  double theta = m_direction - m_rotation;

  double p1 = scaledSpeed * sin(PI / 4 + theta);
  double p2 = scaledSpeed * sin(PI / 4 - theta);

  m_wheelFLSpeed = round(-p1 + m_rotationSpeedDiff / 2.0);
  m_wheelFL->setSpeed(m_wheelFLSpeed);

  m_wheelFRSpeed = round(p2 + m_rotationSpeedDiff / 2.0);
  m_wheelFR->setSpeed(m_wheelFRSpeed);

  m_wheelBLSpeed = round(-p2 + m_rotationSpeedDiff / 2.0);
  m_wheelBL->setSpeed(m_wheelBLSpeed);

  m_wheelBRSpeed = round(p1 + m_rotationSpeedDiff / 2.0);
  m_wheelBR->setSpeed(m_wheelBRSpeed);
}
