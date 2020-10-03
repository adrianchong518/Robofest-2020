#include "control/routines/Routine.h"

#include "hardware/hardware.h"

const control::routines::RoutineID control::routines::getRoutineIDByName(
    String name) {
  for (Routine *const routine : routineList) {
    if (routine->name == name) {
      return routine->id;
    }
  }

  return RoutineID::NONE;
}

void control::routines::FindLineForward::init() {
  hardware::mecanum.moveForward(MECANUM_MOVE_SPEED);
}

bool control::routines::FindLineForward::loop() {
  if (hardware::sensors::irSensors[0].isLineDetected() &&
      hardware::sensors::irSensors[1].isLineDetected()) {
    hardware::mecanum.moveStop();

    return true;
  }

  return false;
}

void control::routines::FindLineBackward::init() {
  hardware::mecanum.moveBackward(MECANUM_MOVE_SPEED);
}

bool control::routines::FindLineBackward::loop() {
  if (hardware::sensors::irSensors[0].isLineDetected() &&
      hardware::sensors::irSensors[1].isLineDetected()) {
    hardware::mecanum.moveStop();

    return true;
  }

  return false;
}

void control::routines::FindEdgeBackward::init() {
  hardware::mecanum.moveBackward(MECANUM_MOVE_SPEED);
}

bool control::routines::FindEdgeBackward::loop() {
  if (hardware::sensors::irSensors[2].isEdgeDetected() &&
      hardware::sensors::irSensors[3].isEdgeDetected()) {
    hardware::mecanum.moveStop();

    return true;
  }

  return false;
}

void control::routines::FindEdgeLeft::init() {
  hardware::servos::setGuideLeft(false);
  hardware::servos::setGuideRight(false);
  data.positionState = 0;

  hardware::mecanum.setIsGyroEnabled(true);
  hardware::mecanum.setDirection(-PI / 2);
  hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
}

bool control::routines::FindEdgeLeft::loop() {
  if (hardware::sensors::irSensors[0].isEdgeDetected() &&
      hardware::sensors::irSensors[2].isEdgeDetected()) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    hardware::servos::setGuideLeft(true);
    hardware::mecanum.setIsGyroEnabled(false);
    delay(100);

    hardware::mecanum.setMotorsSpeeds(-MECANUM_PULL_SPEED * 0.9,
                                      -MECANUM_PULL_SPEED * 0.9,
                                      MECANUM_PULL_SPEED, MECANUM_PULL_SPEED);
    delay(300);

    hardware::mecanum.moveStop();
    data.positionState = 1;

    return true;
  }

  return false;
}

void control::routines::FindEdgeRight::init() {
  hardware::servos::setGuideLeft(false);
  hardware::servos::setGuideRight(false);
  data.positionState = 0;

  hardware::mecanum.setIsGyroEnabled(true);
  hardware::mecanum.setDirection(PI / 2);
  hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
}

bool control::routines::FindEdgeRight::loop() {
  if (hardware::sensors::irSensors[1].isEdgeDetected() &&
      hardware::sensors::irSensors[3].isEdgeDetected()) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    hardware::servos::setGuideRight(true);
    hardware::mecanum.setIsGyroEnabled(false);
    delay(100);

    hardware::mecanum.setMotorsSpeeds(MECANUM_PULL_SPEED * 0.9,
                                      MECANUM_PULL_SPEED * 0.9,
                                      -MECANUM_PULL_SPEED, -MECANUM_PULL_SPEED);
    delay(300);

    hardware::mecanum.moveStop();
    data.positionState = 2;

    return true;
  }

  return false;
}

void control::routines::FindBallForward::init() {
  hardware::mecanum.moveForward(MECANUM_MOVE_SPEED);
}

bool control::routines::FindBallForward::loop() {
  if (hardware::sensors::isLaserBlocked) {
    hardware::mecanum.moveStop();

    return true;
  }

  return false;
}

void control::routines::FindBallSideward::init() {
  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(20);
    hardware::rail.setTarget(0);
  } else {
    hardware::turnTable.setTargetDeg(-20);
    hardware::rail.setTarget(-1);
  }
}

bool control::routines::FindBallSideward::loop() {
  if ((data.positionState == 0 || data.positionState == 1) &&
      hardware::sensors::isBallDetected[1]) {
    hardware::rail.setTarget(hardware::rail.getLocation());
    hardware::servos::setHolderRight(true);
    data.isBallInHolderR = true;

    LOG_DEBUG("<BALL-SIDE>\tFound ball under RIGHT HOLDER");

    return true;
  }

  if ((data.positionState == 0 || data.positionState == 2) &&
      hardware::sensors::isBallDetected[0]) {
    hardware::rail.setTarget(hardware::rail.getLocation());
    hardware::servos::setHolderLeft(true);
    data.isBallInHolderL = true;

    LOG_DEBUG("<BALL-SIDE>\tFound ball under LEFT HOLDER");

    return true;
  }

  return false;
}

void control::routines::HitBall1::init() {
  double delta = atan2(155, DISTANCE_D + 569);

  hardware::rail.setTargetMM(-DISTANCE_C_RAIL_POS);
  hardware::turnTable.setTargetDeg(degrees(delta));

  m_stage = 0;
}

bool control::routines::HitBall1::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HIT_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall2::init() {
  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(-20);
    hardware::rail.setTarget(-1);
  } else if (data.positionState == 2) {
    hardware::turnTable.setTargetDeg(20);
    hardware::rail.setTarget(0);
  }

  m_stage = 0;
}

bool control::routines::HitBall2::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached()) {
    if (data.positionState == 1) {
      hardware::servos::setHolderRight(false);
      delay(1000);

      double delta = atan2(130, DISTANCE_D + 569);

      hardware::turnTable.setTargetDeg(degrees(delta));
      hardware::rail.setTarget(-HOLDER_RIGHT_OFFSET);
    } else if (data.positionState == 2) {
      hardware::servos::setHolderLeft(false);
      delay(1000);

      double delta = -atan2(130, DISTANCE_D + 569);

      hardware::turnTable.setTargetDeg(degrees(delta));
      hardware::rail.setTarget(HOLDER_LEFT_OFFSET);
    }

    m_stage = 1;
  } else if (m_stage == 1 && hardware::rail.isTargetReached() &&
             hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HIT_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 2;
  } else if (m_stage == 2 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall3::init() {
  double delta = -atan2(155, DISTANCE_D + 569);

  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS);
  hardware::turnTable.setTargetDeg(degrees(delta));

  m_stage = 0;
}

bool control::routines::HitBall3::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HIT_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall4::init() {
  double delta = atan2(195, 1188);

  hardware::rail.setTargetMM(-DISTANCE_C_RAIL_POS);
  hardware::turnTable.setTargetDeg(degrees(delta));

  m_stage = 0;
}

bool control::routines::HitBall4::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HIT_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall5::init() {}

bool control::routines::HitBall5::loop() { return true; }