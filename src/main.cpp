#include <Arduino.h>

#include <Wire.h>
#include <Servo.h>

#include <PID.h>
#include <StepperDegree.h>
#include <StepperRail.h>
#include <GY53.h>
#include <JY901.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include "Constants.h"
#include "Motor.h"

StepperRail rail(PIN_RAIL_PUL, PIN_RAIL_DIR, PIN_RAIL_HOME_R, 1);
StepperDegree turnTable(PIN_TURN_CW, PIN_TURN_CCW, PIN_TURN_HOME,
                        TURN_HOME_THRESHOLD, 1);

Servo guideLeft;
Servo guideRight;
Servo holderLeft;
Servo holderRight;
Servo measureServo;

Motor hitterMotor(PIN_HITTER_MOTOR_INA, PIN_HITTER_MOTOR_INB,
                  PIN_HITTER_MOTOR_PWM);
Motor wheelFL(PIN_WHEEL_FL_INA, PIN_WHEEL_FL_INB, PIN_WHEEL_FL_PWM);
Motor wheelFR(PIN_WHEEL_FR_INA, PIN_WHEEL_FR_INB, PIN_WHEEL_FR_PWM);
Motor wheelBL(PIN_WHEEL_BL_INA, PIN_WHEEL_BL_INB, PIN_WHEEL_BL_PWM);
Motor wheelBR(PIN_WHEEL_BR_INA, PIN_WHEEL_BR_INB, PIN_WHEEL_BR_PWM);

PID hitterPID(HITTER_PID_KP, HITTER_PID_KI, HITTER_PID_KD, HITTER_PID_MIN,
              HITTER_PID_MAX);
// TODO Add movement PID control

GY53 irDistance(&SERIAL_IR_DISTANCE, SERIAL_IR_DISTANCE_BAUDRATE);

hd44780_I2Cexp lcd(I2C_LCD_ADDR);

// Encoder IC
uint16_t readEncoderLocation(uint8_t pin_encoderOE) {
  uint16_t location;

  digitalWrite(pin_encoderOE, LOW);

  digitalWrite(PIN_ENCODER_SEL, LOW);
  location = PIN_ENCODER_BUS << 8;
  digitalWrite(PIN_ENCODER_SEL, HIGH);
  location |= PIN_ENCODER_BUS;

  digitalWrite(pin_encoderOE, HIGH);

  return location;
}

void resetEncoderLocation(uint8_t pin_encoderRST) {
  digitalWrite(pin_encoderRST, LOW);
  delay(1);
  digitalWrite(pin_encoderRST, HIGH);
}

void setup() {
  // Disable Interrupts
  noInterrupts();

  // Serial
  Serial.begin(115200);

  // IR Sensors
  pinMode(PIN_IR_FL, INPUT);
  pinMode(PIN_IR_FR, INPUT);
  pinMode(PIN_IR_BL, INPUT);
  pinMode(PIN_IR_BR, INPUT);

  // Laser Photoresistor
  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);

  // Gyroscope
  SERIAL_GYROSCOPE.begin(9600);

  // Encoder
  pinMode(PIN_ENCODER_SEL, OUTPUT);
  digitalWrite(PIN_ENCODER_SEL, LOW);

  pinMode(PIN_HITTER_ENCODER_OE, OUTPUT);
  digitalWrite(PIN_HITTER_ENCODER_OE, HIGH);

  pinMode(PIN_HITTER_ENCODER_RST, OUTPUT);
  digitalWrite(PIN_HITTER_ENCODER_RST, HIGH);

  pinMode(PIN_MEASURE_ENCODER_OE, OUTPUT);
  digitalWrite(PIN_MEASURE_ENCODER_OE, HIGH);

  pinMode(PIN_MEASURE_ENCODER_RST, OUTPUT);
  digitalWrite(PIN_MEASURE_ENCODER_RST, HIGH);

  // Generate clock output
  pinMode(PIN_ENCODER_CLK, OUTPUT);
  TCCR2A = bit(WGM21) | bit(COM2A0);
  TCCR2B = bit(CS20);
  OCR2A = 0;

  // Set up PORT
  DDR_ENCODER_BUS = 0x00;
  PORT_ENCODER_BUS = 0x00;

  // Rail
  rail.setStepLimitEnabled(true);
  rail.setStepLimitMM(RAIL_LOWER_LIMIT_MM, RAIL_UPPER_LIMIT_MM);

  // Turn Table
  turnTable.setPulseWidth(TURN_PULSE_WIDTH);
  turnTable.setStepLimitEnabled(true);
  turnTable.setStepLimitDeg(TURN_LOWER_LIMIT_DEG, TURN_UPPER_LIMIT_DEG);

  // Servos
  guideLeft.attach(PIN_GUIDE_LEFT);
  guideRight.attach(PIN_GUIDE_RIGHT);
  holderLeft.attach(PIN_HOLDER_LEFT);
  holderRight.attach(PIN_HOLDER_RIGHT);
  measureServo.attach(PIN_MEASURE_SERVO);

  // TODO PID Control

  // LCD
  int lcdBeginStatus = lcd.begin(LCD_NUM_COLS, LCD_NUM_ROWS);
  if (lcdBeginStatus) {
    hd44780::fatalError(lcdBeginStatus);
  }

  // Buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // Start button
  pinMode(PIN_START_BUTTON, INPUT_PULLUP);

  // Reenable Interrupts
  interrupts();
}

void loop() {}