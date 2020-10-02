#include "hardware/Mecanum.h"

#include "hardware/servos.h"
#include "hardware/interface.h"

hardware::Mecanum::Mecanum(Motor *const wheelFL, Motor *const wheelFR,
                           Motor *const wheelBL, Motor *const wheelBR)
    : PID(MECANUM_ROT_PID_KP, MECANUM_ROT_PID_KI, MECANUM_ROT_PID_KD,
          MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX),
      m_wheelFL(wheelFL),
      m_wheelFR(wheelFR),
      m_wheelBL(wheelBL),
      m_wheelBR(wheelBR) {
  m_isTargetLimitEnabled = true;
  m_targetLowerLimit = -180;
  m_targetUpperLimit = 180;

  SERIAL_GYROSCOPE.begin(SERIAL_GYROSCOPE_BAUDRATE);
}

hardware::Mecanum::~Mecanum() {}

void hardware::Mecanum::update() {
  if (m_isEnabled) {
    if (m_isGyroEnabled) {
      while (SERIAL_GYROSCOPE.available()) {
        JY901.CopeSerialData(SERIAL_GYROSCOPE.read());
      }

      m_rotation =
          (double)-JY901.stcAngle.Angle[2] / 32768 * PI - m_rotationOffset;
      if (m_rotation > PI) {
        m_rotation -= 2 * PI;
      } else if (m_rotation < -PI) {
        m_rotation += 2 * PI;
      }

      m_rotationSpeedDiff = round(calculatePID(m_rotation));
    } else {
      m_rotation = 0;
    }
    setMotorsSpeeds();
  }
}

void hardware::Mecanum::stop() { setIsEnabled(false); }

void hardware::Mecanum::moveForward(const unsigned int speed) {
  if (hardware::servos::isGuideLeftExtended) {
    setIsGyroEnabled(false);
    setDirection(radians(10));
  } else if (hardware::servos::isGuideRightExtended) {
    setIsGyroEnabled(false);
    setDirection(radians(-10));
  } else {
    setIsGyroEnabled(true);
    setDirection(0);
  }
  setSpeed(speed);
}

void hardware::Mecanum::moveBackward(const unsigned int speed) {
  if (hardware::servos::isGuideLeftExtended) {
    setIsGyroEnabled(false);
    setDirection(radians(170));
  } else if (hardware::servos::isGuideRightExtended) {
    setIsGyroEnabled(false);
    setDirection(radians(-170));
  } else {
    setIsGyroEnabled(true);
    setDirection(PI);
  }
  setSpeed(speed);
}

void hardware::Mecanum::moveStop() {
  if (hardware::servos::isGuideLeftExtended) {
    setIsGyroEnabled(false);
    setDirection(PI / 2);
    setSpeed(MECANUM_STOP_PULL_SPEED);
  } else if (hardware::servos::isGuideRightExtended) {
    setIsGyroEnabled(false);
    setDirection(-PI / 2);
    setSpeed(MECANUM_STOP_PULL_SPEED);
  } else {
    setIsGyroEnabled(true);
    setDirection(0);
    setSpeed(0);
  }
}

void hardware::Mecanum::findRotationOffset() {
  LOG_INFO("<Mecanum>\tFinding Rotation Offset...");

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
  LOG_INFO("<Mecanum>\tRotation Offset: " + String(degrees(m_rotationOffset)));
}

void hardware::Mecanum::setSpeed(const unsigned int speed) {
  m_speed = constrain(speed, 0, 255);
  LOG_DEBUG("<Mecanum>\tSpeed (" + String(m_speed) + ") Set");
}

void hardware::Mecanum::setDirection(const double direction) {
  m_direction = direction;

  while (m_direction > PI) {
    m_direction -= 2 * PI;
  }

  while (m_direction < -PI) {
    m_direction += 2 * PI;
  }

  LOG_DEBUG("<Mecanum>\tDirection (" + String(degrees(m_direction)) + ") Set");
}

void hardware::Mecanum::setRotationSpeedDiff(const int rotationSpeedDiff) {
  m_rotationSpeedDiff =
      constrain(rotationSpeedDiff, MECANUM_ROT_DIFF_MIN, MECANUM_ROT_DIFF_MAX);
}

double hardware::Mecanum::getRotation() { return degrees(m_rotation); }

void hardware::Mecanum::setTarget(double rotationTarget) {
  while (rotationTarget > PI) {
    rotationTarget -= 2 * PI;
  }

  while (rotationTarget < -PI) {
    rotationTarget += 2 * PI;
  }

  PID::setTarget(rotationTarget);
  LOG_DEBUG("<Mecanum>\tRotation Target (" + String(degrees(rotationTarget)) +
            ") Set");
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
  double theta = m_direction;

  double p1 = scaledSpeed * sin(PI / 4 + theta);
  double p2 = scaledSpeed * sin(PI / 4 - theta);

  m_wheelFLSpeed = round(-p1 - m_rotationSpeedDiff / 2.0);
  m_wheelFL->setSpeed(m_wheelFLSpeed);

  m_wheelFRSpeed = round(p2 - m_rotationSpeedDiff / 2.0);
  m_wheelFR->setSpeed(m_wheelFRSpeed);

  m_wheelBLSpeed = round(-p2 - m_rotationSpeedDiff / 2.0);
  m_wheelBL->setSpeed(m_wheelBLSpeed);

  m_wheelBRSpeed = round(p1 - m_rotationSpeedDiff / 2.0);
  m_wheelBR->setSpeed(m_wheelBRSpeed);
}

void hardware::Mecanum::setMotorsSpeeds(const int wheelFLSpeed,
                                        const int wheelFRSpeed,
                                        const int wheelBLSpeed,
                                        const int wheelBRSpeed) {
  m_wheelFLSpeed = wheelFLSpeed;
  m_wheelFL->setSpeed(m_wheelFLSpeed);

  m_wheelFRSpeed = wheelFRSpeed;
  m_wheelFR->setSpeed(m_wheelFRSpeed);

  m_wheelBLSpeed = wheelBLSpeed;
  m_wheelBL->setSpeed(m_wheelBLSpeed);

  m_wheelBRSpeed = wheelBRSpeed;
  m_wheelBR->setSpeed(m_wheelBRSpeed);
}

bool hardware::Mecanum::isEnabled() { return m_isEnabled; }

void hardware::Mecanum::setIsEnabled(const bool isEnabled) {
  LOG_DEBUG("<Mecanum>\t" + String(isEnabled ? "Enabled" : "Disabled"));

  m_isEnabled = isEnabled;

  if (!m_isEnabled) {
    setSpeed(0);
    setRotationSpeedDiff(0);
    setMotorsSpeeds();
  }
}

bool hardware::Mecanum::isGyroEnabled() { return m_isGyroEnabled; }

void hardware::Mecanum::setIsGyroEnabled(const bool isGyroEnabled) {
  LOG_DEBUG("<Mecanum>\tGyroscope " +
            String(isGyroEnabled ? "Enabled" : "Disabled"));

  m_isGyroEnabled = isGyroEnabled;

  if (!m_isGyroEnabled) {
    setRotationSpeedDiff(0);
    setMotorsSpeeds();
  }
}

double hardware::Mecanum::calculateError(double reading) {
  double error = m_target - reading;

  if (error > PI) {
    error -= 2 * PI;
  } else if (error < -PI) {
    error += 2 * PI;
  }

  return error;
}