#include "control/manual.h"

#include "hardware/hardware.h"

String control::manual::input = "";

void control::manual::loop() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      parseInput();
      input = "";
    } else if (inChar == '\b') {
      input.remove(input.length() - 1);
    } else if (inChar == 'x') {
      hardware::stopAll();
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
    hardware::servos::setHolderLeft(!hardware::servos::isHolderLeftExtended);
  } else if (input.startsWith("hr")) {
    hardware::servos::setHolderRight(!hardware::servos::isHolderRightExtended);
  } else if (input.startsWith("gl")) {
    hardware::servos::setGuideLeft(!hardware::servos::isGuideLeftExtended);
  } else if (input.startsWith("gr")) {
    hardware::servos::setGuideRight(!hardware::servos::isGuideRightExtended);
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
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::hardware(const String &command) {
  if (command == "c") {
    hardware::calibrate();
  } else if (command == "h") {
    hardware::defaultPosition();
  } else if (command == "cont") {
    hardware::isHardwareLoopUpdating = true;
    LOG_INFO("<Hardware> Loop Continued");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::rail(const String &command) {
  if (command.startsWith("d ")) {
    hardware::rail.setTargetMM(command.substring(2).toDouble());
  } else if (command.startsWith("s ")) {
    hardware::rail.setTarget(command.substring(2).toInt());
  } else if (command.startsWith("lr")) {
    LOG_INFO("<Rail> Location: " + String(hardware::rail.getLocationMM()) +
             " mm (" + String(hardware::rail.getLocation()) + " steps)");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::turnTable(const String &command) {
  if (command.startsWith("d ")) {
    hardware::turnTable.setTargetDeg(command.substring(2).toDouble());
  } else if (command.startsWith("s ")) {
    hardware::turnTable.setTarget(command.substring(2).toInt());
  } else if (command.startsWith("lr")) {
    LOG_INFO("<Turn Table> Location: " +
             String(hardware::turnTable.getLocationDeg()) + " deg (" +
             String(hardware::turnTable.getLocation()) + " steps)");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::ballHitter(const String &command) {
  if (command.startsWith("set ")) {
    hardware::ballHitter.setTarget(command.substring(4).toDouble());
  } else if (command.startsWith("hit ")) {
    double highPos = command.substring(4, command.indexOf(' ', 4)).toDouble();
    double lowPos = command.substring(command.indexOf(' ', 4)).toDouble();
    hardware::ballHitter.hit(highPos, lowPos);
  } else if (command.startsWith("lr")) {
    LOG_INFO("<Ball Hitter> Encoder Reading: " +
             String(hardware::encoders::hitterEncoderLocation /
                    (double)HITTER_ENCODER_STEP_PER_DEG) +
             " (" + hardware::encoders::hitterEncoderLocation + ")");
  } else if (command.startsWith("rst")) {
    hardware::encoders::resetLocation(PIN_HITTER_ENCODER_RST);
    LOG_INFO("<Ball Hitter> Encoder Reset");
  } else if (command.startsWith("pid")) {
    LOG_INFO("<Ball Hitter> PID Constants: " + String(hardware::ballHitter.Kp) +
             " | " + String(hardware::ballHitter.Ki) + " | " +
             String(hardware::ballHitter.Kd));
  } else if (command.startsWith("kp ")) {
    double Kp = command.substring(3).toDouble();
    hardware::ballHitter.Kp = Kp;
    LOG_DEBUG("<Ball Hitter> PID Kp (" + String(Kp) + ") Set");
  } else if (command.startsWith("ki ")) {
    double Ki = command.substring(3).toDouble();
    hardware::ballHitter.Ki = Ki;
    LOG_DEBUG("<Ball Hitter> PID Ki (" + String(Ki) + ") Set");
  } else if (command.startsWith("kd ")) {
    double Kd = command.substring(3).toDouble();
    hardware::ballHitter.Kd = Kd;
    LOG_DEBUG("<Ball Hitter> PID Kd (" + String(Kd) + ") Set");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::mecanum(const String &command) {
  if (command.startsWith("s ")) {
    hardware::mecanum.setSpeed(command.substring(2).toInt());
  } else if (command.startsWith("d ")) {
    hardware::mecanum.setDirection(radians(command.substring(2).toDouble()));
  } else if (command.startsWith("r ")) {
    hardware::mecanum.setTarget(radians(command.substring(2).toDouble()));
  } else if (command.startsWith("ms")) {
    int wheelFLSpeed, wheelFRSpeed, wheelBLSpeed, wheelBRSpeed;
    hardware::mecanum.getMotorsSpeeds(wheelFLSpeed, wheelFRSpeed, wheelBLSpeed,
                                      wheelBRSpeed);
    LOG_INFO("<Mecanum> Wheels Speeds: " + String(wheelFLSpeed) + " | " +
             String(wheelFRSpeed) + " | " + String(wheelBLSpeed) + " | " +
             String(wheelBRSpeed));
  } else if (command.startsWith("rr")) {
    LOG_INFO("<Mecanum> Rotation: " + String(hardware::mecanum.getRotation()));
  } else if (command.startsWith("gt")) {
    hardware::mecanum.isGyroEnabled = !hardware::mecanum.isGyroEnabled;
    LOG_INFO("<Mecanum> Gyroscope " +
             String(hardware::mecanum.isGyroEnabled ? "Enabled" : "Disabled"));
  } else if (command.startsWith("pid")) {
    LOG_INFO("<Mecanum> PID Constants: " + String(hardware::mecanum.Kp) +
             " | " + String(hardware::mecanum.Ki) + " | " +
             String(hardware::mecanum.Kd));
  } else if (command.startsWith("kp ")) {
    double Kp = command.substring(3).toDouble();
    hardware::mecanum.Kp = Kp;
    LOG_DEBUG("<Mecanum> PID Kp (" + String(Kp) + ") Set");
  } else if (command.startsWith("ki ")) {
    double Ki = command.substring(3).toDouble();
    hardware::mecanum.Ki = Ki;
    LOG_DEBUG("<Mecanum> PID Ki (" + String(Ki) + ") Set");
  } else if (command.startsWith("kd ")) {
    double Kd = command.substring(3).toDouble();
    hardware::mecanum.Kd = Kd;
    LOG_DEBUG("<Mecanum> PID Kd (" + String(Kd) + ") Set");
  } else if (command.startsWith("enable")) {
    hardware::mecanum.isEnabled = !hardware::mecanum.isEnabled;
    LOG_INFO("<Mecanum> " +
             String(hardware::mecanum.isEnabled ? "Enabled" : "Disabled"));
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::measureDistance(const String &command) {
  if (command.startsWith("servo")) {
    hardware::servos::setMeasureServo(
        !hardware::servos::isMeasureServoExtended);
  } else if (command.startsWith("lr")) {
    LOG_INFO("<Measuring Encoder> Reading: " +
             String(hardware::encoders::measureEncoderLocation /
                    (double)MEASURE_ENCODER_STEP_PER_MM) +
             " (" + hardware::encoders::measureEncoderLocation + ")");
  } else if (command.startsWith("rst")) {
    hardware::encoders::resetLocation(PIN_MEASURE_ENCODER_RST);
    LOG_INFO("<Measuring Encoder> Reset");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::irSensors(const String &command) {
  if (command.startsWith("fl")) {
    LOG_INFO("<IR Sensor> FL: " + String(analogRead(PIN_IR_FL)) + " | " +
             String(hardware::sensors::isBlackDetected(PIN_IR_FL)));
  } else if (command.startsWith("fr")) {
    LOG_INFO("<IR Sensor> FR: " + String(analogRead(PIN_IR_FR)) + " | " +
             String(hardware::sensors::isBlackDetected(PIN_IR_FR)));
  } else if (command.startsWith("bl")) {
    LOG_INFO("<IR Sensor> BL: " + String(analogRead(PIN_IR_BL)) + " | " +
             String(hardware::sensors::isBlackDetected(PIN_IR_BL)));
  } else if (command.startsWith("br")) {
    LOG_INFO("<IR Sensor> BR: " + String(analogRead(PIN_IR_BR)) + " | " +
             String(hardware::sensors::isBlackDetected(PIN_IR_BR)));
  } else if (command.startsWith("tr")) {
    LOG_INFO("<IR Sensor> Threshold: " +
             String(hardware::sensors::irThreshold));
  } else if (command.startsWith("ts ")) {
    hardware::sensors::irThreshold = command.substring(3).toInt();
    LOG_INFO("<IR Sensor> Threshold Set (" +
             String(hardware::sensors::irThreshold) + ")");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::laserPhotoresistor(const String &command) {
  if (command.startsWith("r")) {
    LOG_INFO("<Laser Photoresistor> Reading: " +
             String(analogRead(PIN_LASER_PHOTORESISTOR)) + " | " +
             String(hardware::sensors::isLaserBlocked()) + "(" +
             String(hardware::sensors::laserPOTThreshold) + ")");
  } else if (command.startsWith("ts ")) {
    hardware::sensors::laserPOTThreshold = command.substring(3).toInt();
    LOG_INFO("<Laser Photoresistor> Threshold Set (" +
             String(hardware::sensors::laserPOTThreshold) + ")");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::irDistanceSensor(const String &command) {
  if (command.startsWith("dr")) {
    LOG_INFO("<IR Distance> Reading: " +
             String(hardware::sensors::irDistance.getDistance()));
  } else if (command.startsWith("mr")) {
    LOG_INFO("<IR Distance> Mode :" +
             String(hardware::sensors::irDistance.getMode()));
  } else if (command.startsWith("ms ")) {
    hardware::sensors::irDistance.setMode((byte)command.substring(3).toInt());
    LOG_INFO("<IR Distance> Mode Set (" + command.substring(3) + ")");
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}

void control::manual::lcd(const String &command) {
  if (command.startsWith("print ")) {
    hardware::interface::lcd.print(command.substring(6));
  } else if (command.startsWith("nl")) {
    hardware::interface::lcd.setCursor(0, 1);
  } else if (command.startsWith("clr")) {
    hardware::interface::lcd.clear();
  } else {
    LOG_ERROR("Invalid command: " + input);
  }
}
