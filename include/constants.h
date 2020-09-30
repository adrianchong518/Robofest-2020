#ifndef CONSTANTS_H
#define CONSTANTS_H

// Pin Definitions

#define PIN_RAIL_PUL 43
#define PIN_RAIL_DIR 44
#define PIN_RAIL_HOME_L 47
#define PIN_RAIL_HOME_R A0

#define PIN_TURN_CW 53
#define PIN_TURN_CCW 52
#define PIN_TURN_HOME A11

#define PIN_IR_FL A12
#define PIN_IR_FR A8
#define PIN_IR_BL A14
#define PIN_IR_BR A9

#define PIN_GUIDE_LEFT 27
#define PIN_GUIDE_RIGHT 12

#define PIN_HOLDER_LEFT 48
#define PIN_HOLDER_RIGHT 49

#define PIN_MEASURE_SERVO 26

#define PIN_HITTER_MOTOR_INA 45
#define PIN_HITTER_MOTOR_INB 46
#define PIN_HITTER_MOTOR_PWM 4

#define PIN_WHEEL_FL_INA 24
#define PIN_WHEEL_FL_INB 25
#define PIN_WHEEL_FL_PWM 2

#define PIN_WHEEL_FR_INA 11
#define PIN_WHEEL_FR_INB 9
#define PIN_WHEEL_FR_PWM 8

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
#define SERIAL_GYROSCOPE_BAUDRATE 9600

#define SERIAL_IR_DISTANCE Serial2
#define SERIAL_IR_DISTANCE_BAUDRATE 115200

#define I2C_LCD_ADDR 0x3F

#define PIN_BUZZER 13

#define PORT_SW_BTN PORTF
#define DDR_SW_BTN DDRF
#define PIN_SW_BTN PINF

#define BITS_DIP_SW 4  // 4 - 7
#define PIN_BUTTON_1 A1
#define PIN_BUTTON_2 A2
#define PIN_BUTTON_3 A3

// Parameters

#define RAIL_STEP_PER_MM 12.5
#define RAIL_PULSE_WIDTH 560

#define TURN_STEP_PER_DEG 50
#define TURN_HOME_THRESHOLD 800
#define TURN_PULSE_WIDTH 200
#define TURN_LOWER_LIMIT_DEG -30
#define TURN_UPPER_LIMIT_DEG 30

#define HITTER_PID_KP 0.5
#define HITTER_PID_KI 0.001
#define HITTER_PID_KD 3.5
#define HITTER_PID_MIN -70
#define HITTER_PID_MAX 70

#define HITTER_TARGET_DEG_MIN -20  // TODO Need Data
#define HITTER_TARGET_DEG_MAX 100  // TODO Need Data
#define HITTER_DEG_ALLOWED_ERROR 3

#define MECANUM_ROT_PID_KP 800
#define MECANUM_ROT_PID_KI 0.1
#define MECANUM_ROT_PID_KD 5000

#define MECANUM_ROT_DIFF_MIN -300  // -255*2
#define MECANUM_ROT_DIFF_MAX 300   // 255*2

#define MECANUM_MOVE_SPEED 255
#define MECANUM_PULL_SPEED 110

#define GUIDE_LEFT_RETRACTED_POS 90
#define GUIDE_LEFT_EXTENDED_POS 140

#define GUIDE_RIGHT_RETRACTED_POS 100
#define GUIDE_RIGHT_EXTENDED_POS 50

#define HOLDER_LEFT_RETRACTED_POS 120
#define HOLDER_LEFT_EXTENDED_POS 60

#define HOLDER_RIGHT_RETRACTED_POS 70
#define HOLDER_RIGHT_EXTENDED_POS 130

#define MEASURE_RETRACTED_POS 120
#define MEASURE_EXTENDED_POS 95

#define HITTER_ENCODER_STEP_PER_DEG 7.5

#define MEASURE_ENCODER_STEP_PER_MM 91.76

#define IR_FL_LINE_THRESHOLD 400
#define IR_FL_EDGE_THRESHOLD 300

#define IR_FR_LINE_THRESHOLD 300
#define IR_FR_EDGE_THRESHOLD 200

#define IR_BL_LINE_THRESHOLD 300
#define IR_BL_EDGE_THRESHOLD 200

#define IR_BR_LINE_THRESHOLD 300
#define IR_BR_EDGE_THRESHOLD 200

#define LASER_THRESHOLD 500

#define LASER_THRESHOLD 500  // TODO Need Data

#define LCD_NUM_COLS 16
#define LCD_NUM_ROWS 2

#endif
