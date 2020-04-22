#include "control/manual.h"

#include "hardware/hardware.h"

String control::manual::input = "";

void control::manual::loop() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      parseInput();
      input = "";
    } else {
      input.concat(inChar);
    }
  }
}

void control::manual::parseInput() {
  if (input.length() == 0) {
    return;
  }

  if (input.startsWith("r ")) {
    double target = input.substring(2).toDouble();
    rail(target);
  } else if (input.startsWith("tt ")) {
    double target = input.substring(3).toDouble();
    turnTable(target);
  } else if (input.startsWith("bh ")) {
    double target = input.substring(3).toDouble();
    ballHitter(target);
  } else if (input.startsWith("hl")) {
    hardware::servos::setGuideLeft(!hardware::servos::isGuideLeftExtented);
  } else if (input.startsWith("hr")) {
    hardware::servos::setGuideRight(!hardware::servos::isGuideRightExtented);
  } else if (input.startsWith("gl")) {
    hardware::servos::setHolderLeft(!hardware::servos::isHolderLeftExtented);
  } else if (input.startsWith("gr")) {
    hardware::servos::setHolderRight(!hardware::servos::isHolderRightExtented);
  } else if (input.startsWith("ms")) {
    hardware::servos::setMeasureServo(
        !hardware::servos::isMeasureServoExtented);
  }
}

void control::manual::rail(double target) {
  hardware::Rail::CODES returnCode = hardware::rail.setTargetMM(target);

  switch (returnCode) {
    case hardware::Rail::NO_ERROR:
      Serial.println("Rail Target (" + String(target) + ") Set");
      break;

    case hardware::Rail::ERROR_TARGET_EXCEEDS_LIMIT:
      Serial.println("Rail Target (" + String(target) + ") Exceeds Limit");
      break;

    default:
      Serial.println("Unknown Error");
      break;
  }
}

void control::manual::turnTable(double target) {
  hardware::TurnTable::CODES returnCode =
      hardware::turnTable.setTargetDeg(target);

  switch (returnCode) {
    case hardware::TurnTable::NO_ERROR:
      Serial.println("Turn Table Target (" + String(target) + ") Set");
      break;

    case hardware::TurnTable::ERROR_TARGET_EXCEEDS_LIMIT:
      Serial.println("Turn Table Target (" + String(target) +
                     ") Exceeds Limit");
      break;

    default:
      break;
  }
}

void control::manual::ballHitter(double target) {
  PID::CODES returnCode = hardware::hitterPID.setTarget(target);
}
