#include "hardware/hardware.h"

hardware::Rail hardware::rail(PIN_RAIL_PUL, PIN_RAIL_DIR, PIN_RAIL_HOME_L,
                              PIN_RAIL_HOME_R, RAIL_STEP_PER_MM);
hardware::TurnTable hardware::turnTable(PIN_TURN_CW, PIN_TURN_CCW,
                                        PIN_TURN_HOME, TURN_HOME_THRESHOLD,
                                        TURN_STEP_PER_DEG);

hardware::Motor hardware::hitterMotor(PIN_HITTER_MOTOR_INA,
                                      PIN_HITTER_MOTOR_INB,
                                      PIN_HITTER_MOTOR_PWM);

hardware::Motor hardware::wheelFL(PIN_WHEEL_FL_INA, PIN_WHEEL_FL_INB,
                                  PIN_WHEEL_FL_PWM);
hardware::Motor hardware::wheelFR(PIN_WHEEL_FR_INA, PIN_WHEEL_FR_INB,
                                  PIN_WHEEL_FR_PWM);
hardware::Motor hardware::wheelBL(PIN_WHEEL_BL_INA, PIN_WHEEL_BL_INB,
                                  PIN_WHEEL_BL_PWM);
hardware::Motor hardware::wheelBR(PIN_WHEEL_BR_INA, PIN_WHEEL_BR_INB,
                                  PIN_WHEEL_BR_PWM);
hardware::Mecanum hardware::mecanum(&wheelFL, &wheelFR, &wheelBL, &wheelBR);

PID hardware::hitterPID(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD,
                        HITTER_PID_MIN, HITTER_PID_MAX);

GY53 hardware::irDistance(&SERIAL_IR_DISTANCE, SERIAL_IR_DISTANCE_BAUDRATE);

hd44780_I2Cexp hardware::lcd(I2C_LCD_ADDR);

void hardware::init() {
  // IR Sensors
  pinMode(PIN_IR_FL, INPUT);
  pinMode(PIN_IR_FR, INPUT);
  pinMode(PIN_IR_BL, INPUT);
  pinMode(PIN_IR_BR, INPUT);

  // Laser Photoresistor
  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);

  // Encoder
  hardware::encoders::init();

  // Turn Table
  turnTable.setPulseWidth(TURN_PULSE_WIDTH);
  turnTable.setStepLimitEnabled(true);
  turnTable.setStepLimitDeg(TURN_LOWER_LIMIT_DEG, TURN_UPPER_LIMIT_DEG);

  // Servos
  hardware::servos::init();

  // Hitter PID
  hitterPID.setTargetLimitEnabled(true);
  hitterPID.setTargetLimit(HITTER_TARGET_DEG_MIN * HITTER_ENCODER_STEP_PER_DEG,
                           HITTER_TARGET_DEG_MAX * HITTER_ENCODER_STEP_PER_DEG);
  hitterPID.setAllowedError(HITTER_DEG_ALLOWED_ERROR *
                            HITTER_ENCODER_STEP_PER_DEG);

  // LCD
  int lcdBeginStatus = lcd.begin(LCD_NUM_COLS, LCD_NUM_ROWS);
  if (lcdBeginStatus) {
    hd44780::fatalError(lcdBeginStatus);
  }

  // Buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // Buttons & DIP Switches
  DDR_SW_BTN = 0x00;
  PORT_SW_BTN = 0x00;
}

void hardware::calibrate() { mecanum.findRotationOffset(); }

void hardware::defaultPosition() {
  rail.home();
  turnTable.home();
  hardware::servos::defaultPosition();
  hardware::encoders::defaultPosition();
}

void hardware::loop() {
  rail.update();
  turnTable.update();
  hardware::encoders::loop();
}
