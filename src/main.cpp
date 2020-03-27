#include <Arduino.h>

#include <Wire.h>
#include <Servo.h>

#include <PID.h>
#include <StepperDegree.h>
#include <StepperRail.h>
#include <GY53.h>
#include <JY901.h>

#include "Motor.h"

#define PIN_RAIL_PUL 45
#define PIN_RAIL_DIR 46
#define PIN_RAIL_HOME_L 47
#define PIN_RAIL_HOME_R A0

#define PIN_TURN_CW 48
#define PIN_TURN_CCW 49
#define PIN_TURN_HOME A11
#define TURN_HOME_THRESHOLD 700

#define PIN_IR_FL A12
#define PIN_IR_FR A10
#define PIN_IR_BL A14
#define PIN_IR_BR A15

#define PIN_GUIDE_LEFT 27
#define PIN_GUIDE_RIGHT 12

#define PIN_HOLDER_LEFT 52
#define PIN_HOLDER_RIGHT 53

#define PIN_MEASURE_SERVO 26

#define PIN_HITTER_MOTOR_INA 50
#define PIN_HITTER_MOTOR_INB 51
#define PIN_HITTER_MOTOR_PWM 4

#define HITTER_PID_KP 1
#define HITTER_PID_KI 0
#define HITTER_PID_KD 0
#define HITTER_PID_MIN -255
#define HITTER_PID_MAX 255

#define PIN_WHEEL_FL_INA 24
#define PIN_WHEEL_FL_INB 25
#define PIN_WHEEL_FL_PWM 2

#define PIN_WHEEL_FR_INA 11
#define PIN_WHEEL_FR_INB 8
#define PIN_WHEEL_FR_PWM 9

#define PIN_WHEEL_BL_INA 22
#define PIN_WHEEL_BL_INB 23
#define PIN_WHEEL_BL_PWM 3

#define PIN_WHEEL_BR_INA 7
#define PIN_WHEEL_BR_INB 5
#define PIN_WHEEL_BR_PWM 6

#define PORT_ENCODER_BUS PORTC
#define DDR_ENCODER_BUS DDRC
#define PIN_ENCODER_BUS PINC
#define PIN_ENCODER_CLK 10
#define PIN_ENCODER_SEL 38

#define PIN_HITTER_ENCODER_OE 39
#define PIN_HITTER_ENCODER_RST 40

#define PIN_MEASURE_ENCODER_OE 41
#define PIN_MEASURE_ENCODER_RST 42

#define PIN_LASER_PHOTORESISTOR A13

#define SERIAL_GYROSCOPE Serial3

#define SERIAL_IR_DISTANCE Serial2
#define SERIAL_IR_DISTANCE_BAUDRATE 115200

#define PIN_BUZZER 13

#define PIN_START_BUTTON A1

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