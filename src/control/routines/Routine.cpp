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
  delay(500);

  hardware::mecanum.setIsGyroEnabled(true);
  hardware::mecanum.setDirection(-PI / 2);
  hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);

  hardware::turnTable.setTargetDeg(-10);
  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_LEFT);

  m_stage = 0;
}

bool control::routines::FindEdgeLeft::loop() {
  if (m_stage == 0 && hardware::sensors::irSensors[0].isEdgeDetected() &&
      hardware::sensors::irSensors[2].isEdgeDetected()) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    hardware::servos::setGuideLeft(true);
    hardware::mecanum.setIsGyroEnabled(false);
    delay(200);

    hardware::mecanum.setMotorsSpeeds(-MECANUM_PULL_SPEED * 0.9,
                                      -MECANUM_PULL_SPEED * 0.9,
                                      MECANUM_PULL_SPEED, MECANUM_PULL_SPEED);
    delay(300);

    hardware::mecanum.moveStop();
    data.positionState = 1;

    m_stage = 1;
  } else if (m_stage == 1 && hardware::rail.isTargetReached()) {
    return true;
  }

  return false;
}

void control::routines::FindEdgeRight::init() {
  hardware::servos::setGuideLeft(false);
  hardware::servos::setGuideRight(false);
  data.positionState = 0;
  delay(500);

  hardware::mecanum.setIsGyroEnabled(true);
  hardware::mecanum.setDirection(PI / 2);
  hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);

  hardware::turnTable.setTargetDeg(10);
  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_RIGHT);

  m_stage = 0;
}

bool control::routines::FindEdgeRight::loop() {
  if (m_stage == 0 && hardware::sensors::irSensors[1].isEdgeDetected() &&
      hardware::sensors::irSensors[3].isEdgeDetected()) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    hardware::servos::setGuideRight(true);
    hardware::mecanum.setIsGyroEnabled(false);
    delay(200);

    hardware::mecanum.setMotorsSpeeds(MECANUM_PULL_SPEED * 0.9,
                                      MECANUM_PULL_SPEED * 0.9,
                                      -MECANUM_PULL_SPEED, -MECANUM_PULL_SPEED);
    delay(300);

    hardware::mecanum.moveStop();
    data.positionState = 2;

    m_stage = 1;
  } else if (m_stage == 1 && hardware::rail.isTargetReached()) {
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
    hardware::mecanum.moveForward(MECANUM_COMPENSATE_SPEED);
    m_stage = 1;
  } else if (m_stage == 1 &&
             hardware::encoders::readLocation(PIN_MEASURE_ENCODER_OE) -
                     m_measureStart >=
                 BALL_FWD_COMPENSATION * MEASURE_ENCODER_STEP_PER_MM) {
    hardware::mecanum.moveStop();
    hardware::servos::setMeasureServo(false);
    return true;
  }

  return false;
}

void control::routines::FindBallSideward::init() {
  data.isBallInHolderL = false;
  data.isBallInHolderR = false;

  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(10);
    hardware::rail.setTarget(0);
  } else if (data.positionState == 2) {
    hardware::turnTable.setTargetDeg(-10);
    hardware::rail.setTarget(-1);
  }

  m_stage = 0;
}

bool control::routines::FindBallSideward::loop() {
  if (m_stage == 0) {
    if (data.positionState == 1 && hardware::sensors::isBallDetected[1]) {
      hardware::rail.setTargetMM(hardware::rail.getLocationMM() -
                                 BALL_SIDE_COMPENSATION);
      data.isBallInHolderR = true;

      LOG_DEBUG("<BALL-SIDE>\tFound ball under RIGHT HOLDER");

      m_stage = 1;
    }

    if (data.positionState == 2 && hardware::sensors::isBallDetected[0]) {
      hardware::rail.setTargetMM(hardware::rail.getLocationMM() +
                                 BALL_SIDE_COMPENSATION);
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
    delay(500);

    return true;
  }

  return false;
}

void control::routines::FindBall5::init() {
  hardware::mecanum.setIsGyroEnabled(true);
  hardware::mecanum.setSpeed(0);
  hardware::mecanum.setTarget(PI - STARTING_ORIENTATION);

  m_stage = 0;
}

bool control::routines::FindBall5::loop() {
  if (m_runningSubroutine != nullptr) {
    if (m_runningSubroutine->loop()) {
      m_runningSubroutine = nullptr;
    }
  }

  if (m_stage == 0 && hardware::mecanum.isTargetReached()) {
    hardware::mecanum.setDirection(-PI / 2);
    hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
    m_stage = 1;
  } else if (m_stage == 1 &&
             (hardware::sensors::irSensors[0].isEdgeDetected() ||
              hardware::sensors::irSensors[2].isEdgeDetected())) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    delay(100);

    hardware::mecanum.setDirection(PI / 2);
    hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
    hardware::mecanum.setMotorsSpeeds();
    delay(200);

    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setTarget(-PI / 2 - STARTING_ORIENTATION);
    m_stage = 2;
  } else if (m_stage == 2 && hardware::mecanum.isTargetReached()) {
    m_stage = 3;
  } else if (m_stage == 3) {
    m_runningSubroutine = new FindBallForward;
    m_runningSubroutine->init();
    m_stage = 4;
  } else if (m_stage == 4 && m_runningSubroutine == nullptr) {
    m_runningSubroutine = new FindBall5Sideward;
    m_runningSubroutine->init();
    m_stage = 5;
  } else if (m_stage == 5 && m_runningSubroutine == nullptr) {
    hardware::mecanum.setDirection(-PI / 2);
    hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
    m_stage = 6;
  } else if (m_stage == 6 &&
             (hardware::sensors::irSensors[0].isEdgeDetected() ||
              hardware::sensors::irSensors[2].isEdgeDetected())) {
    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setMotorsSpeeds();
    delay(100);

    hardware::mecanum.setDirection(PI / 2);
    hardware::mecanum.setSpeed(MECANUM_MOVE_SPEED);
    hardware::mecanum.setMotorsSpeeds();
    delay(200);

    hardware::mecanum.setSpeed(0);
    hardware::mecanum.setTarget(-STARTING_ORIENTATION);
    m_stage = 7;
  } else if (m_stage == 7 && hardware::mecanum.isTargetReached()) {
    m_runningSubroutine = new FindEdgeLeft;
    m_runningSubroutine->init();
    m_stage = 8;
  } else if (m_stage == 8 && m_runningSubroutine == nullptr) {
    hardware::mecanum.findRotationOffset();
    hardware::mecanum.setTarget(0);
    return true;
  }

  return false;
}

void control::routines::FindBall5Sideward::init() {
  data.isBallInHolderL = false;
  data.isBallInHolderR = false;

  m_isBallFoundUnderLeftHolder = hardware::sensors::isBallDetected[0];
  hardware::turnTable.setTargetDeg(0);
  hardware::rail.setTarget(-1);

  m_startTime = millis();

  if (m_isBallFoundUnderLeftHolder) {
    LOG_DEBUG("<BALL-5-SIDE>\tBall Already Under Left Holder");
  }

  m_stage = 0;
}

bool control::routines::FindBall5Sideward::loop() {
  if (m_stage == 0) {
    Serial.print(hardware::sensors::irDistanceSensors[0].getDistance());
    Serial.print(" ");
    Serial.println(hardware::sensors::irDistanceSensors[1].getDistance());

    if (m_isBallFoundUnderLeftHolder) {
      if (!hardware::sensors::isBallDetected[0]) {
        LOG_DEBUG("<BALL-5-SIDE>\tBall Edge Found");

        if (hardware::rail.getLocationMM() > BALL_SIDE_COMPENSATION) {
          hardware::rail.setTargetMM(hardware::rail.getLocationMM() -
                                     BALL_SIDE_COMPENSATION);

          LOG_DEBUG("<BALL-5-SIDE>\tFound ball under LEFT HOLDER");
          data.isBallInHolderL = true;
          m_stage = 1;
        } else {
          LOG_DEBUG("<BALL-5-SIDE>\tIgnoring Ball");
          m_isBallFoundUnderLeftHolder = false;
        }
      }
    } else {
      if (hardware::sensors::isBallDetected[0]) {
        if (millis() - m_startTime <= BALL_5_SIDE_BUFFER_TIME) {
          LOG_DEBUG("<BALL-5-SIDE>\tBall Already Under Left Holder");
          m_isBallFoundUnderLeftHolder = true;
        } else {
          LOG_DEBUG("<BALL-5-SIDE>\tFound ball under LEFT HOLDER");
          hardware::rail.setTargetMM(hardware::rail.getLocationMM() +
                                     BALL_SIDE_COMPENSATION);
          data.isBallInHolderL = true;
          m_stage = 1;
        }
      } else if (hardware::sensors::isBallDetected[1]) {
        hardware::rail.setTargetMM(hardware::rail.getLocationMM() +
                                   BALL_SIDE_COMPENSATION);
        LOG_DEBUG("<BALL-5-SIDE>\tFound ball under RIGHT HOLDER");
        data.isBallInHolderR = true;
        m_stage = 1;
      }
    }
  } else if (m_stage == 1 && hardware::rail.isTargetReached()) {
    if (data.isBallInHolderL) {
      hardware::servos::setHolderLeft(true);
    } else if (data.isBallInHolderR) {
      hardware::servos::setHolderRight(true);
    }
    delay(500);

    return true;
  }

  return false;
}

void control::routines::HitBall1::init() {
  double delta = atan2(140, DISTANCE_D + 569);
  long target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_1_OFFSET;

  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_LEFT);
  hardware::turnTable.setTarget(target);

  m_stage = 0;
}

bool control::routines::HitBall1::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_BOTTLE_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall2::init() {
  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(-10);
    hardware::rail.setTarget(-1);
  } else if (data.positionState == 2) {
    hardware::turnTable.setTargetDeg(10);
    hardware::rail.setTarget(0);
  }

  m_stage = 0;
}

bool control::routines::HitBall2::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached()) {
    if (data.positionState == 1) {
      hardware::servos::setHolderRight(false);
      data.isBallInHolderR = false;
      delay(1000);

      double delta = atan2(145, DISTANCE_D + 569);
      long target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_1_OFFSET;

      hardware::turnTable.setTarget(target);
      hardware::rail.setTarget(-HOLDER_RIGHT_OFFSET);
    } else if (data.positionState == 2) {
      hardware::servos::setHolderLeft(false);
      data.isBallInHolderL = false;
      delay(1000);

      double delta = -atan2(145, DISTANCE_D + 569);
      long target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_3_OFFSET;

      hardware::turnTable.setTarget(target);
      hardware::rail.setTarget(HOLDER_LEFT_OFFSET);
    }

    m_stage = 1;
  } else if (m_stage == 1 && hardware::rail.isTargetReached() &&
             hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HOLE_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 2;
  } else if (m_stage == 2 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall3::init() {
  double delta = -atan2(140, DISTANCE_D + 569);
  long target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_3_OFFSET;

  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_RIGHT);
  hardware::turnTable.setTarget(target);

  m_stage = 0;
}

bool control::routines::HitBall3::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_BOTTLE_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall4::init() {
  double delta = atan2(215, 1188);
  long target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_4_OFFSET;

  hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_LEFT);
  hardware::turnTable.setTarget(target);

  m_stage = 0;
}

bool control::routines::HitBall4::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached() &&
      hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_BALL_4_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 1;
  } else if (m_stage == 1 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}

void control::routines::HitBall5::init() {
  if (data.positionState == 1) {
    hardware::turnTable.setTargetDeg(-10);
    if (data.isBallInHolderL) {
      hardware::rail.setTarget(DISTANCE_C_RAIL_POS_LEFT * RAIL_STEP_PER_MM -
                               HOLDER_LEFT_OFFSET);
    } else if (data.isBallInHolderR) {
      hardware::rail.setTarget(-1);
    }
  } else if (data.positionState == 2) {
    hardware::turnTable.setTargetDeg(10);
    if (data.isBallInHolderL) {
      hardware::rail.setTarget(0);
    } else if (data.isBallInHolderR) {
      hardware::rail.setTarget(DISTANCE_C_RAIL_POS_RIGHT * RAIL_STEP_PER_MM +
                               HOLDER_RIGHT_OFFSET);
    }
  }

  m_stage = 0;
}

bool control::routines::HitBall5::loop() {
  if (m_stage == 0 && hardware::rail.isTargetReached()) {
    long target;
    if (data.positionState == 1) {
      if (data.isBallInHolderL) {
        hardware::servos::setHolderLeft(false);
        data.isBallInHolderL = false;
        delay(1000);

        double delta = atan2(195, DISTANCE_D + 569);
        target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_1_OFFSET;
        hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_LEFT);
      } else if (data.isBallInHolderR) {
        hardware::servos::setHolderRight(false);
        data.isBallInHolderR = false;
        delay(1000);

        double delta = atan2(145, DISTANCE_D + 569);
        target = degrees(delta) * TURN_STEP_PER_DEG + TURN_BALL_1_OFFSET;
        hardware::rail.setTarget(-HOLDER_RIGHT_OFFSET);
      }
    } else if (data.positionState == 2) {
      if (data.isBallInHolderL) {
        hardware::servos::setHolderLeft(false);
        data.isBallInHolderL = false;
        delay(1000);

        double delta = -atan2(145, DISTANCE_D + 569);
        target = degrees(delta) * TURN_STEP_PER_DEG;
        hardware::rail.setTarget(HOLDER_LEFT_OFFSET);
      } else if (data.isBallInHolderR) {
        hardware::servos::setHolderRight(false);
        data.isBallInHolderR = false;
        delay(1000);

        double delta = -atan2(195, DISTANCE_D + 569);
        target = degrees(delta) * TURN_STEP_PER_DEG;
        hardware::rail.setTargetMM(DISTANCE_C_RAIL_POS_RIGHT);
      }
    }
    hardware::turnTable.setTarget(target);

    m_stage = 1;
  } else if (m_stage == 1 && hardware::rail.isTargetReached() &&
             hardware::turnTable.isTargetReached()) {
    hardware::ballHitter.hit(HITTER_HOLE_HIGH_POS, HITTER_HIT_LOW_POS);
    m_stage = 2;
  } else if (m_stage == 2 && hardware::ballHitter.getHitStage() == 0) {
    return true;
  }

  return false;
}