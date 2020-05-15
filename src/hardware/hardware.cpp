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

hd44780_I2Cexp hardware::lcd;  //(I2C_LCD_ADDR);

void hardware::init() {
  // Sensors
  hardware::sensors::init();

  // Encoder
  hardware::encoders::init();

  // Rail
  rail.setPulseWidth(RAIL_PULSE_WIDTH);

  // Turn Table
  turnTable.setPulseWidth(TURN_PULSE_WIDTH);
  turnTable.setStepLimitEnabled(true);
  turnTable.setStepLimitDeg(TURN_LOWER_LIMIT_DEG, TURN_UPPER_LIMIT_DEG);

  // Servos
  hardware::servos::init();

  // LCD
  int lcdBeginStatus = lcd.begin(LCD_NUM_COLS, LCD_NUM_ROWS);
  if (lcdBeginStatus) {
    hd44780::fatalError(lcdBeginStatus);
  }

  // Buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // Buttons & DIP Switches
  DDR_SW_BTN = 0x00 | (DDR_SW_BTN & 0x01);
  PORT_SW_BTN = 0x00 | (PORT_SW_BTN & 0x01);
}

void hardware::calibrate() {
  // Sensors
  hardware::sensors::calibrate();

  // Mecanum (Gyroscope)
  mecanum.findRotationOffset();
}

void hardware::defaultPosition() {
  turnTable.home(TURN_PULSE_WIDTH);
  turnTable.setTargetDeg(30);
  while (!turnTable.isTargetReached()) {
    turnTable.update();
  }

  rail.home(RAIL_PULSE_WIDTH);

  hardware::servos::defaultPosition();
  hardware::encoders::defaultPosition();
}

void hardware::loop() {
  rail.update();
  turnTable.update();
  ballHitter.update(hardware::encoders::hitterEncoderLocation);
  mecanum.update();
  hardware::encoders::loop();
  hardware::sensors::loop();
}

byte hardware::readDIPSwitches() { return ~PIN_SW_BTN >> BITS_DIP_SW; }
