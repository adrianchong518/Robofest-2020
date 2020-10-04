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

void control::routines::FindLineReverse::init() {
  hardware::mecanum.moveBackward(MECANUM_MOVE_SPEED);
}

bool control::routines::FindLineReverse::loop() {
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
  hardware::servos::setMeasureServo(true);
  delay(200);
  hardware::encoders::resetLocation(PIN_MEASURE_ENCODER_RST);

  hardware::mecanum.moveForward(MECANUM_MOVE_SPEED);
  m_stage = 0;
}

bool control::routines::FindBallForward::loop() {
  if (m_stage == 0 && hardware::sensors::isLaserBlocked) {
    m_measureStart = hardware::encoders::readLocation(PIN_MEASURE_ENCODER_OE);
    hardware::mecanum.moveForward(50);
    m_stage = 1;
  } else if (m_stage == 1 &&
             hardware::encoders::readLocation(PIN_MEASURE_ENCODER_OE) -
                     m_measureStart >=
                 7 * MEASURE_ENCODER_STEP_PER_MM) {
    hardware::mecanum.moveStop();
    hardware::servos::setMeasureServo(false);
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

  m_stage = 0;
}

bool control::routines::FindBallSideward::loop() {
  if (m_stage == 0) {
    if ((data.positionState == 0 || data.positionState == 1) &&
        hardware::sensors::isBallDetected[1]) {
      long compensation = data.positionState == 0
                              ? BALL_SIDE_COMPENSATION * RAIL_STEP_PER_MM
                              : -BALL_SIDE_COMPENSATION * RAIL_STEP_PER_MM;
      hardware::rail.setTarget(hardware::rail.getLocation() + compensation);
      data.isBallInHolderR = true;

      LOG_DEBUG("<BALL-SIDE>\tFound ball under RIGHT HOLDER");

      m_stage = 1;
    }

    if ((data.positionState == 0 || data.positionState == 2) &&
        hardware::sensors::isBallDetected[0]) {
      long compensation = BALL_SIDE_COMPENSATION * RAIL_STEP_PER_MM;
      hardware::rail.setTarget(hardware::rail.getLocation() + compensation);
      data.isBallInHolderL = true;

      LOG_DEBUG("<BALL-SIDE>\tFound ball under LEFT HOLDER");

      m_stage = 1;
    }
  } else if (m_stage == 1 && hardware::rail.isTargetReached()) {
    if (data.isBallInHolderL) {
      hardware::servos::setHolderLeft(true);
    } else if (data.isBallInHolderR) {
      hardware::servos::setHolderRight(true);
    }

    return true;
  }

  return false;
}

void control::routines::FindBall5::init() {
  hardware::mecanum.moveForward(MECANUM_MOVE_SPEED);

  m_stage = 0;
  timer = millis();
}

bool control::routines::FindBall5::loop() {
  if (m_runningSubroutine != nullptr) {
    if (m_runningSubroutine->loop()) {
      m_runningSubroutine = nullptr;
    }
  }

  if (m_stage == 0 && millis() - timer > BALL_5_OFFSET_TIME) {
    hardware::servos::setGuideRight(false);
    hardware::mecanum.moveStop();
    hardware::mecanum.setTarget(-PI / 2);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::mecanum.isTargetReached()) {
    m_runningSubroutine = new FindBallForward;
    m_runningSubroutine->init();
    m_stage = 2;
  } else if (m_stage == 2 && m_runningSubroutine == nullptr) {
    m_runningSubroutine = new FindBallSideward;
    m_runningSubroutine->init();
    m_stage = 3;
  } else if (m_stage == 3 && m_runningSubroutine == nullptr) {
    hardware::mecanum.setTarget(0);
    m_stage = 4;
  } else if (m_stage == 4 && hardware::mecanum.isTargetReached()) {
    return true;
  }

  return false;
}

void control::routines::HitBall1::init() {
  double delta = atan2(145, DISTANCE_D + 569);

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
  double delta = -atan2(145, DISTANCE_D + 569);

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

void control::routines::HitBall5::init() {
  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(-20);
    if (data.isBallInHolderL) {
      hardware::rail.setTargetMM(-DISTANCE_C_RAIL_POS - HOLDER_LEFT_OFFSET);
    } else if (data.isBallInHolderR) {
      hardware::rail.setTarget(-1);
    }
  } else if (data.positionState == 2) {
    hardware::turnTable.setTargetDeg(20);
    if (data.isBallInHolderL) {
      hardware::rail.setTarget(0);
    } else if (data.isBallInHolderR) {
      hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS + HOLDER_RIGHT_OFFSET);
    }
  }

  m_stage = 0;
}

bool control::routines::HitBall5::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached()) {
    double delta;
    if (data.positionState == 1) {
      if (data.isBallInHolderL) {
        delta = -atan2(195, DISTANCE_D + 569);
        hardware::rail.setTargetMM(-DISTANCE_C_RAIL_POS);
      } else if (data.isBallInHolderR) {
        delta = -atan2(130, DISTANCE_D + 569);
        hardware::rail.setTarget(-HOLDER_RIGHT_OFFSET);
      }
    } else if (data.positionState == 2) {
      if (data.isBallInHolderL) {
        delta = atan2(130, DISTANCE_D + 569);
        hardware::rail.setTarget(HOLDER_LEFT_OFFSET);
      } else if (data.isBallInHolderR) {
        delta = atan2(195, DISTANCE_D + 569);
        hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS);
      }
    }
    hardware::turnTable.setTargetDeg(degrees(delta));

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