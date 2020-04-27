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
    rail(input.substring(2).toDouble());
  } else if (input.startsWith("tt ")) {
    turnTable(input.substring(3).toDouble());
  } else if (input.startsWith("bh ")) {
    ballHitter(input.substring(3));
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
  } else if (input.startsWith("ir ")) {
    irSensors(input.substring(3));
  } else if (input.startsWith("lr")) {
    Serial.println("Laser Photoresistor: " +
                   String(analogRead(PIN_LASER_PHOTORESISTOR)));
  } else if (input.startsWith("lcd ")) {
    lcd(input.substring(4));
  } else {
    Serial.println("Invalid command: " + input);
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
      Serial.println("Unknown Error");
      break;
  }
}

void control::manual::ballHitter(String command) {
  if (command.startsWith("set ")) {
    double target = command.substring(4).toDouble();
    PID::CODES returnCode = hardware::ballHitter.setTarget(target);

    switch (returnCode) {
      case PID::NO_ERROR:
        Serial.println("Ball Hitter Target (" + String(target) + ") Set");
        break;

      case PID::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("Ball Hitter Target (" + String(target) +
                       ") Exceeds Limit");
        break;

      default:
        Serial.println("Unknown Error");
        break;
    }
  } else if (command.startsWith("hit ")) {
    double highPos = command.substring(4, command.indexOf(' ', 4)).toDouble();
    double lowPos = command.substring(command.indexOf(' ', 4)).toDouble();
    PID::CODES returnCode = hardware::ballHitter.hit(highPos, lowPos);

    switch (returnCode) {
      case PID::NO_ERROR:
        Serial.println("Ball Hitter Hit Targets (" + String(highPos) + "," +
                       String(lowPos) + ") Set");
        break;

      case PID::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("Ball Hitter Hit Targets (" + String(highPos) + "," +
                       String(lowPos) + ") Exceeds Limit");
        break;

      default:
        break;
    }
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::irSensors(String command) {
  if (command == "fl") {
    Serial.println("IR Sensor FL: " + String(analogRead(PIN_IR_FL)));
  } else if (command == "fr") {
    Serial.println("IR Sensor FR: " + String(analogRead(PIN_IR_FR)));
  } else if (command == "bl") {
    Serial.println("IR Sensor BL: " + String(analogRead(PIN_IR_BL)));
  } else if (command == "br") {
    Serial.println("IR Sensor BR: " + String(analogRead(PIN_IR_BR)));
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::lcd(String command) {
  if (command.startsWith("println ")) {
    hardware::lcd.print(command.substring(8) + '\n');
  } else if (command == "clr") {
    hardware::lcd.clear();
  } else {
    Serial.println("Invalid command: " + input);
  }
}
