#include <Arduino.h>

#include <Wire.h>
#include <Servo.h>

#include <PID.h>
#include <StepperDegree.h>
#include <StepperRail.h>
#include <GY53.h>
#include <JY901.h>

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

// TODO Add LCD

void setup() {
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
  pinMode(PIN_ENCODER_CLK, OUTPUT);
  TCCR2A = bit(WGM21) | bit(COM2A0);
  TCCR2B = bit(CS20);
  OCR2A = 0;

  DDR_ENCODER_BUS = 0x00;
  PORT_ENCODER_BUS = 0x00;

  // TODO Rail

  // TODO Turn Table

  // Servos
  guideLeft.attach(PIN_GUIDE_LEFT);
  guideRight.attach(PIN_GUIDE_RIGHT);
  holderLeft.attach(PIN_HOLDER_LEFT);
  holderRight.attach(PIN_HOLDER_RIGHT);
  measureServo.attach(PIN_MEASURE_SERVO);

  // TODO PID Control
}

void loop() {}