#include "control/commands.h"

#include "hardware/hardware.h"
#include "control/routines/routines.h"

int hardwareCommands(const String &command) {
  if (command == "c") {
    hardware::calibrate();
  } else if (command == "h") {
    hardware::defaultPosition();
  } else if (command == "cont") {
    hardware::isHardwareLoopUpdating = true;
    LOG_INFO("<Hardware>\tLoop Continued");
  } else {
    return -1;
  }

  return 0;
}

int railCommands(const String &command) {
  if (command.startsWith("d ")) {
    hardware::rail.setTargetMM(command.substring(2).toDouble());
  } else if (command.startsWith("s ")) {
    hardware::rail.setTarget(command.substring(2).toInt());
  } else if (command == "lr") {
    LOG_INFO("<Rail>\tLocation: " + String(hardware::rail.getLocationMM()) +
             " mm (" + String(hardware::rail.getLocation()) + " steps)");
  } else if (command == "home") {
    hardware::rail.home(RAIL_PULSE_WIDTH);
  } else {
    return -1;
  }

  return 0;
}

int turnTableCommands(const String &command) {
  if (command.startsWith("d ")) {
    hardware::turnTable.setTargetDeg(command.substring(2).toDouble());
  } else if (command.startsWith("s ")) {
    hardware::turnTable.setTarget(command.substring(2).toInt());
  } else if (command == "lr") {
    LOG_INFO("<Turn Table>\tLocation: " +
             String(hardware::turnTable.getLocationDeg()) + " deg (" +
             String(hardware::turnTable.getLocation()) + " steps)");
  } else if (command == "home") {
    hardware::turnTable.home(TURN_PULSE_WIDTH);
  } else {
    return -1;
  }

  return 0;
}

int ballHitterCommands(const String &command) {
  if (command.startsWith("set ")) {
    hardware::ballHitter.setTarget(command.substring(4).toDouble());
  } else if (command.startsWith("hit ")) {
    double highPos = command.substring(4, command.indexOf(' ', 4)).toDouble();
    double lowPos = command.substring(command.indexOf(' ', 4)).toDouble();
    hardware::ballHitter.hit(highPos, lowPos);
  } else if (command == "lr") {
    LOG_INFO("<Ball Hitter>\tEncoder Reading: " +
             String(hardware::encoders::hitterEncoderLocation /
                    (double)HITTER_ENCODER_STEP_PER_DEG) +
             " (" + hardware::encoders::hitterEncoderLocation + ")");
  } else if (command == "rst") {
    hardware::encoders::resetLocation(PIN_HITTER_ENCODER_RST);
    LOG_INFO("<Ball Hitter>\tEncoder Reset");
  } else if (command == "pid") {
    LOG_INFO(
        "<Ball Hitter>\tPID Constants: " + String(hardware::ballHitter.Kp) +
        " | " + String(hardware::ballHitter.Ki) + " | " +
        String(hardware::ballHitter.Kd));
  } else if (command.startsWith("kp ")) {
    double Kp = command.substring(3).toDouble();
    hardware::ballHitter.Kp = Kp;
    LOG_DEBUG("<Ball Hitter>\tPID Kp (" + String(Kp) + ") Set");
  } else if (command.startsWith("ki ")) {
    double Ki = command.substring(3).toDouble();
    hardware::ballHitter.Ki = Ki;
    LOG_DEBUG("<Ball Hitter>\tPID Ki (" + String(Ki) + ") Set");
  } else if (command.startsWith("kd ")) {
    double Kd = command.substring(3).toDouble();
    hardware::ballHitter.Kd = Kd;
    LOG_DEBUG("<Ball Hitter>\tPID Kd (" + String(Kd) + ") Set");
  } else {
    return -1;
  }

  return 0;
}

int mecanumCommands(const String &command) {
  if (command.startsWith("s ")) {
    hardware::mecanum.setSpeed(command.substring(2).toInt());
  } else if (command.startsWith("d ")) {
    hardware::mecanum.setDirection(radians(command.substring(2).toDouble()));
  } else if (command.startsWith("r ")) {
    hardware::mecanum.setTarget(radians(command.substring(2).toDouble()));
  } else if (command == "ms") {
    int wheelFLSpeed, wheelFRSpeed, wheelBLSpeed, wheelBRSpeed;
    hardware::mecanum.getMotorsSpeeds(wheelFLSpeed, wheelFRSpeed, wheelBLSpeed,
                                      wheelBRSpeed);
    LOG_INFO("<Mecanum>\tWheels Speeds: " + String(wheelFLSpeed) + " | " +
             String(wheelFRSpeed) + " | " + String(wheelBLSpeed) + " | " +
             String(wheelBRSpeed));
  } else if (command == "rr") {
    LOG_INFO("<Mecanum>\tRotation: " + String(hardware::mecanum.getRotation()));
  } else if (command == "gt") {
    hardware::mecanum.setIsGyroEnabled(!hardware::mecanum.isGyroEnabled());
  } else if (command == "c") {
    hardware::mecanum.findRotationOffset();
  } else if (command == "pid") {
    LOG_INFO("<Mecanum>\tPID Constants: " + String(hardware::mecanum.Kp) +
             " | " + String(hardware::mecanum.Ki) + " | " +
             String(hardware::mecanum.Kd));
  } else if (command.startsWith("kp ")) {
    double Kp = command.substring(3).toDouble();
    hardware::mecanum.Kp = Kp;
    LOG_DEBUG("<Mecanum>\tPID Kp (" + String(Kp) + ") Set");
  } else if (command.startsWith("ki ")) {
    double Ki = command.substring(3).toDouble();
    hardware::mecanum.Ki = Ki;
    LOG_DEBUG("<Mecanum>\tPID Ki (" + String(Ki) + ") Set");
  } else if (command.startsWith("kd ")) {
    double Kd = command.substring(3).toDouble();
    hardware::mecanum.Kd = Kd;
    LOG_DEBUG("<Mecanum>\tPID Kd (" + String(Kd) + ") Set");
  } else if (command == "t") {
    hardware::mecanum.setIsEnabled(!hardware::mecanum.isEnabled());
  } else {
    return -1;
  }

  return 0;
}

int servosCommands(const String &command) {
  if (command == "hl") {
    hardware::servos::setHolderLeft(!hardware::servos::isHolderLeftExtended);
  } else if (command == "hr") {
    hardware::servos::setHolderRight(!hardware::servos::isHolderRightExtended);
  } else if (command == "gl") {
    hardware::servos::setGuideLeft(!hardware::servos::isGuideLeftExtended);
  } else if (command == "gr") {
    hardware::servos::setGuideRight(!hardware::servos::isGuideRightExtended);
  } else if (command == "md") {
    hardware::servos::setMeasureServo(
        !hardware::servos::isMeasureServoExtended);
  } else if (command == "home") {
    hardware::servos::defaultPosition();
  } else {
    return -1;
  }

  return 0;
}

int measureDistanceCommands(const String &command) {
  if (command == "lr") {
    LOG_INFO("<Measuring Encoder>\tReading: " +
             String(hardware::encoders::measureEncoderLocation /
                    (double)MEASURE_ENCODER_STEP_PER_MM) +
             " (" + hardware::encoders::measureEncoderLocation + ")");
  } else if (command == "rst") {
    hardware::encoders::resetLocation(PIN_MEASURE_ENCODER_RST);
    LOG_INFO("<Measuring Encoder>\tReset");
  } else {
    return -1;
  }

  return 0;
}

int irSensorsCommands(const String &command) {
  int irSensorIndex;
  if (command.startsWith("fl ")) {
    irSensorIndex = 0;
  } else if (command.startsWith("fr ")) {
    irSensorIndex = 1;
  } else if (command.startsWith("bl ")) {
    irSensorIndex = 2;
  } else if (command.startsWith("br ")) {
    irSensorIndex = 3;
  } else if (command == "vr") {
    for (irSensorIndex = 0; irSensorIndex < 4; irSensorIndex++) {
      LOG_INFO(
          "<IR Sensor>\t" + hardware::sensors::irSensorsNames[irSensorIndex] +
          ": " +
          String(hardware::sensors::irSensors[irSensorIndex].getValue()) +
          " | " +
          String(hardware::sensors::irSensors[irSensorIndex].isLineDetected()) +
          " | " +
          String(hardware::sensors::irSensors[irSensorIndex].isEdgeDetected()));
    }

    return 0;
  } else {
    return -1;
  }

  String subcommand = command.substring(3);
  if (subcommand == "vr") {
    LOG_INFO(
        "<IR Sensor>\t" + hardware::sensors::irSensorsNames[irSensorIndex] +
        ": " + String(hardware::sensors::irSensors[irSensorIndex].getValue()) +
        " | " +
        String(hardware::sensors::irSensors[irSensorIndex].isLineDetected()) +
        " | " +
        String(hardware::sensors::irSensors[irSensorIndex].isEdgeDetected()));
  } else if (subcommand == "tr") {
    LOG_INFO(
        "<IR Sensor>\t" + hardware::sensors::irSensorsNames[irSensorIndex] +
        " Thresholds: " +
        String(hardware::sensors::irSensors[irSensorIndex].getLineThreshold()) +
        " | " +
        String(hardware::sensors::irSensors[irSensorIndex].getEdgeThreshold()));
  } else if (subcommand.startsWith("lts ")) {
    hardware::sensors::irSensors[irSensorIndex].setLineThreshold(
        subcommand.substring(4).toInt());
  } else if (subcommand.startsWith("ets ")) {
    hardware::sensors::irSensors[irSensorIndex].setEdgeThreshold(
        subcommand.substring(4).toInt());
  } else {
    return -1;
  }

  return 0;
}

int laserPhotoresistorCommands(const String &command) {
  if (command == "r") {
    LOG_INFO(
        "<Laser>\t\tReading: " + String(analogRead(PIN_LASER_PHOTORESISTOR)) +
        " | " + String(hardware::sensors::isLaserBlocked) + "(" +
        String(hardware::sensors::laserThreshold) + ")");
  } else if (command.startsWith("ts ")) {
    hardware::sensors::laserThreshold = command.substring(3).toInt();
    LOG_INFO("<Laser>\t\tThreshold Set (" +
             String(hardware::sensors::laserThreshold) + ")");
  } else {
    return -1;
  }

  return 0;
}

int irDistanceSensorCommands(const String &command) {
  if (command == "dr") {
    LOG_INFO("<IR Distance>\tReading: " +
             String(hardware::sensors::irDistance.getDistance()));
  } else if (command == "mr") {
    LOG_INFO("<IR Distance>\tMode :" +
             String(hardware::sensors::irDistance.getMode()));
  } else if (command.startsWith("ms ")) {
    hardware::sensors::irDistance.setMode((byte)command.substring(3).toInt());
    LOG_INFO("<IR Distance>\tMode Set (" + command.substring(3) + ")");
  } else {
    return -1;
  }

  return 0;
}

int lcdCommands(const String &command) {
  if (command.startsWith("print ")) {
    hardware::interface::lcd.print(command.substring(6));
  } else if (command == "nl") {
    hardware::interface::lcd.setCursor(0, 1);
  } else if (command == "clr") {
    hardware::interface::lcd.clear();
  } else {
    return -1;
  }

  return 0;
}

int runCommands(const String &command) {
  if (command.startsWith("routine ")) {
    control::routines::runRoutine(
        control::routines::getRoutineIDByName(command.substring(8)));
  } else if (command.startsWith("seq ")) {
    control::routines::runSeq(command.substring(4).toInt());
  } else {
    return -1;
  }

  return 0;
}

int control::commands::parseInput(const String &command) {
  if (command.length() == 0) {
    return -1;
  }

  if (command.startsWith("h ")) {
    return hardwareCommands(command.substring(2));
  } else if (command.startsWith("r ")) {
    return railCommands(command.substring(2));
  } else if (command.startsWith("tt ")) {
    return turnTableCommands(command.substring(3));
  } else if (command.startsWith("bh ")) {
    return ballHitterCommands(command.substring(3));
  } else if (command.startsWith("m ")) {
    return mecanumCommands(command.substring(2));
  } else if (command.startsWith("servos ")) {
    return servosCommands(command.substring(7));
  } else if (command.startsWith("md ")) {
    return measureDistanceCommands(command.substring(3));
  } else if (command.startsWith("ir ")) {
    return irSensorsCommands(command.substring(3));
  } else if (command.startsWith("lr ")) {
    return laserPhotoresistorCommands(command.substring(3));
  } else if (command.startsWith("ird ")) {
    return irDistanceSensorCommands(command.substring(4));
  } else if (command.startsWith("lcd ")) {
    return lcdCommands(command.substring(4));
  } else if (command.startsWith("run ")) {
    return runCommands(command.substring(4));
  }

  return -1;
}
