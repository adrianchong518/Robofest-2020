#include "control/manual.h"

#include "hardware/hardware.h"

String control::manual::input = "";

void control::manual::loop() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      parseInput();
      input = "";
    } else if (inChar == 'x') {
      hardware::stopAll();
      Serial.println("\n<Hardware> Stopped All");
    } else {
      input.concat(inChar);
    }
  }
}

void control::manual::parseInput() {
  if (input.length() == 0) {
    return;
  }

  if (input.startsWith("h ")) {
    hardware(input.substring(2));
  } else if (input.startsWith("r ")) {
    rail(input.substring(2));
  } else if (input.startsWith("tt ")) {
    turnTable(input.substring(3));
  } else if (input.startsWith("bh ")) {
    ballHitter(input.substring(3));
  } else if (input.startsWith("m ")) {
    mecanum(input.substring(2));
  } else if (input.startsWith("hl")) {
    hardware::servos::setHolderLeft(!hardware::servos::isHolderLeftExtented);
  } else if (input.startsWith("hr")) {
    hardware::servos::setHolderRight(!hardware::servos::isHolderRightExtented);
  } else if (input.startsWith("gl")) {
    hardware::servos::setGuideLeft(!hardware::servos::isGuideLeftExtented);
  } else if (input.startsWith("gr")) {
    hardware::servos::setGuideRight(!hardware::servos::isGuideRightExtented);
  } else if (input.startsWith("md ")) {
    measureDistance(input.substring(3));
  } else if (input.startsWith("ir ")) {
    irSensors(input.substring(3));
  } else if (input.startsWith("lr ")) {
    laserPhotoresistor(input.substring(3));
  } else if (input.startsWith("ird ")) {
    irDistanceSensor(input.substring(4));
  } else if (input.startsWith("lcd ")) {
    lcd(input.substring(4));
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::hardware(const String &command) {
  if (command == "c") {
    Serial.println("<Hardware> Calibrating...");
    hardware::calibrate();
    Serial.println("<Hardware> Calibration Done");
  } else if (command == "h") {
    Serial.println("<Hardware> Homing...");
    hardware::defaultPosition();
    Serial.println("<Hardware> Homing Done");
  } else if (command == "cont") {
    hardware::isHardwareLoopUpdating = true;
    Serial.println("<Hardware> Loop Updating Continued");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::rail(const String &command) {
  if (command.startsWith("d ")) {
    double target = command.substring(2).toDouble();
    hardware::Rail::CODES returnCode = hardware::rail.setTargetMM(target);

    switch (returnCode) {
      case hardware::Rail::NO_ERROR:
        Serial.println("<Rail> Target (" + String(target) + " mm) Set");
        break;

      case hardware::Rail::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Rail> Target (" + String(target) +
                       " mm) Exceeds Limit");
        break;

      default:
        Serial.println("<Rail> Unknown Error");
        break;
    }
  } else if (command.startsWith("s ")) {
    long target = command.substring(2).toInt();
    hardware::Rail::CODES returnCode = hardware::rail.setTarget(target);

    switch (returnCode) {
      case hardware::Rail::NO_ERROR:
        Serial.println("<Rail> Target (" + String(target) + " steps) Set");
        break;

      case hardware::Rail::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Rail> Target (" + String(target) +
                       " steps) Exceeds Limit");
        break;

      default:
        Serial.println("<Rail> Unknown Error");
        break;
    }
  } else if (command.startsWith("lr")) {
    Serial.println(
        "<Rail> Location: " + String(hardware::rail.getLocationMM()) + " mm (" +
        String(hardware::rail.getLocation()) + " steps)");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::turnTable(const String &command) {
  if (command.startsWith("d ")) {
    double target = command.substring(2).toDouble();
    hardware::TurnTable::CODES returnCode =
        hardware::turnTable.setTargetDeg(target);

    switch (returnCode) {
      case hardware::TurnTable::NO_ERROR:
        Serial.println("<Turn Table> Target (" + String(target) + " deg) Set");
        break;

      case hardware::TurnTable::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Turn Table> Target (" + String(target) +
                       " deg) Exceeds Limit");
        break;

      default:
        Serial.println("<Turn Table> Unknown Error");
        break;
    }
  } else if (command.startsWith("s ")) {
    long target = command.substring(2).toInt();
    hardware::TurnTable::CODES returnCode =
        hardware::turnTable.setTarget(target);

    switch (returnCode) {
      case hardware::TurnTable::NO_ERROR:
        Serial.println("<Turn Table> Target (" + String(target) +
                       " steps) Set");
        break;

      case hardware::TurnTable::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Turn Table> Target (" + String(target) +
                       " steps) Exceeds Limit");
        break;

      default:
        Serial.println("<Turn Table> Unknown Error");
        break;
    }
  } else if (command.startsWith("lr")) {
    Serial.println("<Turn Table> Location: " +
                   String(hardware::turnTable.getLocationDeg()) + " deg (" +
                   String(hardware::turnTable.getLocation()) + " steps)");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::ballHitter(const String &command) {
  if (command.startsWith("set ")) {
    double target = command.substring(4).toDouble();
    PID::CODES returnCode = hardware::ballHitter.setTarget(target);

    switch (returnCode) {
      case PID::NO_ERROR:
        Serial.println("<Ball Hitter> Target (" + String(target) + ") Set");
        break;

      case PID::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Ball Hitter> Target (" + String(target) +
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
        Serial.println("<Ball Hitter> Hit Targets (" + String(highPos) + "," +
                       String(lowPos) + ") Set");
        break;

      case PID::ERROR_TARGET_EXCEEDS_LIMIT:
        Serial.println("<Ball Hitter> Hit Targets (" + String(highPos) + "," +
                       String(lowPos) + ") Exceeds Limit");
        break;

      default:
        break;
    }
  } else if (command.startsWith("lr")) {
    Serial.println("<Ball Hitter> Encoder Reading: " +
                   String(hardware::encoders::hitterEncoderLocation /
                          (double)HITTER_ENCODER_STEP_PER_DEG) +
                   " (" + hardware::encoders::hitterEncoderLocation + ")");
  } else if (command.startsWith("rst")) {
    hardware::encoders::resetLocation(PIN_HITTER_ENCODER_RST);
    Serial.println("<Ball Hitter> Encoder Reset");
  } else if (command.startsWith("pid")) {
    Serial.println(
        "<Ball Hitter> PID Constants: " + String(hardware::ballHitter.Kp) +
        " | " + String(hardware::ballHitter.Ki) + " | " +
        String(hardware::ballHitter.Kd));
  } else if (command.startsWith("kp ")) {
    double Kp = command.substring(3).toDouble();
    hardware::ballHitter.Kp = Kp;
    Serial.println("<Ball Hitter> PID Kp (" + String(Kp) + ") Set");
  } else if (command.startsWith("ki ")) {
    double Ki = command.substring(3).toDouble();
    hardware::ballHitter.Ki = Ki;
    Serial.println("<Ball Hitter> PID Ki (" + String(Ki) + ") Set");
  } else if (command.startsWith("kd ")) {
    double Kd = command.substring(3).toDouble();
    hardware::ballHitter.Kd = Kd;
    Serial.println("<Ball Hitter> PID Kd (" + String(Kd) + ") Set");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::mecanum(const String &command) {
  if (command.startsWith("s ")) {
    unsigned int speed = command.substring(2).toInt();
    hardware::mecanum.setSpeed(speed);
    Serial.println("<Mecanum> Speed (" + String(speed) + ") Set");
  } else if (command.startsWith("d ")) {
    double direction = command.substring(2).toDouble();
    hardware::mecanum.setDirection(radians(direction));
    Serial.println("<Mecanum> Direction (" + String(direction) + ") Set");
  } else if (command.startsWith("r ")) {
    double rotation = command.substring(2).toDouble();
    hardware::mecanum.setRotationTarget(radians(rotation));
    Serial.println("<Mecanum> Rotation Target (" + String(rotation) + ") Set");
  } else if (command.startsWith("ms")) {
    int wheelFLSpeed, wheelFRSpeed, wheelBLSpeed, wheelBRSpeed;
    hardware::mecanum.getMotorsSpeeds(wheelFLSpeed, wheelFRSpeed, wheelBLSpeed,
                                      wheelBRSpeed);
    Serial.println("<Mecanum> Wheels Speeds: " + String(wheelFLSpeed) + " | " +
                   String(wheelFRSpeed) + " | " + String(wheelBLSpeed) + " | " +
                   String(wheelBRSpeed));
  } else if (command.startsWith("rr")) {
    Serial.println("<Mecanum> Rotation: " +
                   String(hardware::mecanum.getRotation()));
  } else if (command.startsWith("gt")) {
    hardware::mecanum.m_isGyroEnabled = !hardware::mecanum.m_isGyroEnabled;
    Serial.println(
        "<Mecanum> Gyroscope " +
        String(hardware::mecanum.m_isGyroEnabled ? "Enabled" : "Disabled"));
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::measureDistance(const String &command) {
  if (command.startsWith("servo")) {
    hardware::servos::setMeasureServo(
        !hardware::servos::isMeasureServoExtented);
  } else if (command.startsWith("lr")) {
    Serial.println("<Measuring Encoder> Reading: " +
                   String(hardware::encoders::measureEncoderLocation /
                          (double)MEASURE_ENCODER_STEP_PER_MM) +
                   " (" + hardware::encoders::measureEncoderLocation + ")");
  } else if (command.startsWith("rst")) {
    hardware::encoders::resetLocation(PIN_MEASURE_ENCODER_RST);
    Serial.println("<Measuring Encoder> Reset");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::irSensors(const String &command) {
  if (command.startsWith("fl")) {
    Serial.println("<IR Sensor> FL: " + String(analogRead(PIN_IR_FL)) + " | " +
                   String(hardware::sensors::isBlackDetected(PIN_IR_FL)));
  } else if (command.startsWith("fr")) {
    Serial.println("<IR Sensor> FR: " + String(analogRead(PIN_IR_FR)) + " | " +
                   String(hardware::sensors::isBlackDetected(PIN_IR_FR)));
  } else if (command.startsWith("bl")) {
    Serial.println("<IR Sensor> BL: " + String(analogRead(PIN_IR_BL)) + " | " +
                   String(hardware::sensors::isBlackDetected(PIN_IR_BL)));
  } else if (command.startsWith("br")) {
    Serial.println("<IR Sensor> BR: " + String(analogRead(PIN_IR_BR)) + " | " +
                   String(hardware::sensors::isBlackDetected(PIN_IR_BR)));
  } else if (command.startsWith("tr")) {
    Serial.println("<IR Sensor> Threshold: " +
                   String(hardware::sensors::irThreshold));
  } else if (command.startsWith("ts ")) {
    hardware::sensors::irThreshold = command.substring(3).toInt();
    Serial.println("<IR Sensor> Threshold Set (" +
                   String(hardware::sensors::irThreshold) + ")");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::laserPhotoresistor(const String &command) {
  if (command.startsWith("r")) {
    Serial.println("<Laser Photoresistor> Reading: " +
                   String(analogRead(PIN_LASER_PHOTORESISTOR)) + " | " +
                   String(hardware::sensors::isLaserBlocked()) + "(" +
                   String(hardware::sensors::laserPOTThreshold) + ")");
  } else if (command.startsWith("ts ")) {
    hardware::sensors::laserPOTThreshold = command.substring(3).toInt();
    Serial.println("<Laser Photoresistor> Threshold Set (" +
                   String(hardware::sensors::laserPOTThreshold) + ")");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::irDistanceSensor(const String &command) {
  if (command.startsWith("dr")) {
    Serial.println("<IR Distance> Reading: " +
                   String(hardware::sensors::irDistance.getDistance()));
  } else if (command.startsWith("mr")) {
    Serial.println("<IR Distance> Mode :" +
                   String(hardware::sensors::irDistance.getMode()));
  } else if (command.startsWith("ms ")) {
    hardware::sensors::irDistance.setMode((byte)command.substring(3).toInt());
    Serial.println("<IR Distance> Mode Set (" + command.substring(3) + ")");
  } else {
    Serial.println("Invalid command: " + input);
  }
}

void control::manual::lcd(const String &command) {
  if (command.startsWith("print ")) {
    hardware::lcd.print(command.substring(6));
  } else if (command.startsWith("nl")) {
    hardware::lcd.setCursor(0, 1);
  } else if (command.startsWith("clr")) {
    hardware::lcd.clear();
  } else {
    Serial.println("Invalid command: " + input);
  }
}
