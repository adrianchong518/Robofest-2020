#include "hardware/hardware.h"

#include "constants.h"

hardware::Rail hardware::rail(PIN_RAIL_PUL, PIN_RAIL_DIR, PIN_RAIL_HOME_L,
                              PIN_RAIL_HOME_R, RAIL_STEP_PER_MM);
hardware::TurnTable hardware::turnTable(PIN_TURN_CW, PIN_TURN_CCW,
                                        PIN_TURN_HOME, TURN_HOME_THRESHOLD,
                                        TURN_STEP_PER_DEG);

hardware::Motor hardware::hitterMotor(PIN_HITTER_MOTOR_INA,
                                      PIN_HITTER_MOTOR_INB,
                                      PIN_HITTER_MOTOR_PWM);
hardware::BallHitter hardware::ballHitter(&hitterMotor);

hardware::Motor hardware::wheelFL(PIN_WHEEL_FL_INA, PIN_WHEEL_FL_INB,
                                  PIN_WHEEL_FL_PWM);
hardware::Motor hardware::wheelFR(PIN_WHEEL_FR_INA, PIN_WHEEL_FR_INB,
                                  PIN_WHEEL_FR_PWM);
hardware::Motor hardware::wheelBL(PIN_WHEEL_BL_INA, PIN_WHEEL_BL_INB,
                                  PIN_WHEEL_BL_PWM);
hardware::Motor hardware::wheelBR(PIN_WHEEL_BR_INA, PIN_WHEEL_BR_INB,
                                  PIN_WHEEL_BR_PWM);
hardware::Mecanum hardware::mecanum(&wheelFL, &wheelFR, &wheelBL, &wheelBR);

bool hardware::isHardwareLoopUpdating = true;

void hardware::init() {
  LOG_INFO("<Hardware>\tInit Start...");

  interface::init();
  sensors::init();
  encoders::init();
  servos::init();

  rail.setPulseWidth(RAIL_PULSE_WIDTH);

  turnTable.setPulseWidth(TURN_PULSE_WIDTH);
  turnTable.setStepLimitEnabled(true);
  turnTable.setStepLimitDeg(TURN_LOWER_LIMIT_DEG, TURN_UPPER_LIMIT_DEG);

  mecanum.isEnabled = bitRead(interface::operationMode, 2);

  LOG_INFO("<Hardware>\tInit Complete");
}

void hardware::calibrate() {
  LOG_INFO("<Hardware>\tCalibration Start...");

  sensors::calibrate();
  mecanum.findRotationOffset();

  LOG_INFO("<Hardware>\tCalibration Complete");
}

void hardware::defaultPosition() {
  LOG_INFO("<Hardware>\tHoming Start...");

  servos::defaultPosition();
  turnTable.home(TURN_PULSE_WIDTH);
  rail.home(RAIL_PULSE_WIDTH);
  encoders::defaultPosition();

  LOG_INFO("<Hardware>\tHoming Complete");
}

void hardware::stopAll() {
  mecanum.stop();
  rail.stop();
  turnTable.stop();
  ballHitter.stop();

  isHardwareLoopUpdating = false;
  LOG_INFO("<Hardware>\tStopped Loop");
}

void hardware::loop() {
  if (isHardwareLoopUpdating) {
    encoders::loop();
    sensors::loop();
    rail.update();
    turnTable.update();
    ballHitter.update(hardware::encoders::hitterEncoderLocation);
    mecanum.update();
  }
}
