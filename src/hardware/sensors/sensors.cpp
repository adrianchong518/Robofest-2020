#include "hardware/sensors/sensors.h"

#include "constants.h"
#include "hardware/interface.h"

hardware::sensors::IRSensor hardware::sensors::irSensors[4] = {
    IRSensor(PIN_IR_FL), IRSensor(PIN_IR_FR), IRSensor(PIN_IR_BL),
    IRSensor(PIN_IR_BR)};

GY53 hardware::sensors::irDistanceSensors[2] = {
    GY53(&SERIAL_IR_DISTANCE_L, SERIAL_IR_DISTANCE_L_BAUDRATE),
    GY53(&SERIAL_IR_DISTANCE_R, SERIAL_IR_DISTANCE_R_BAUDRATE)};

uint16_t hardware::sensors::irDistanceSensorThreshold[2] = {
    IR_DISTANCE_L_THRESHOLD, IR_DISTANCE_R_THRESHOLD};
bool hardware::sensors::isBallDetected[2] = {false, false};

int hardware::sensors::laserThreshold = LASER_THRESHOLD;
bool hardware::sensors::isLaserBlocked = false;

void hardware::sensors::init() {
  LOG_DEBUG("<Sensors>\tInitialising...");

  irDistanceSensors[0].init();
  irDistanceSensors[1].init();

  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);
}

void hardware::sensors::calibrateIRSensors() {
  LOG_INFO("<IR Sensors>\tCalibrating...");

  int tableValues[4];
  int lineValues[4];
  int edgeValues[4];

  // Table Value
  LOG_INFO("<IR Sensors>\tALL on TABLE");
  interface::lcd.setCursor(0, 0);
  interface::lcd.print("IR ALL on TABLE ");
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;

  for (int i = 0; i < 10; i++) {
    tableValues[0] += analogRead(PIN_IR_FL);
    tableValues[1] += analogRead(PIN_IR_FR);
    tableValues[2] += analogRead(PIN_IR_BL);
    tableValues[3] += analogRead(PIN_IR_BR);

    delay(100);
  }
  tableValues[0] /= 10;
  tableValues[1] /= 10;
  tableValues[2] /= 10;
  tableValues[3] /= 10;

  // Line Value F
  LOG_INFO("<IR Sensors>\tF on LINE");
  interface::lcd.setCursor(0, 0);
  interface::lcd.print("IR F on LINE    ");
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;

  for (int i = 0; i < 10; i++) {
    lineValues[0] += analogRead(PIN_IR_FL);
    lineValues[1] += analogRead(PIN_IR_FR);

    delay(100);
  }
  lineValues[0] /= 10;
  lineValues[1] /= 10;

  // Line Value B
  LOG_INFO("<IR Sensors>\tB on LINE");
  interface::lcd.setCursor(0, 0);
  interface::lcd.print("IR B on LINE    ");
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  for (int i = 0; i < 10; i++) {
    lineValues[2] += analogRead(PIN_IR_BL);
    lineValues[3] += analogRead(PIN_IR_BR);

    delay(100);
  }
  lineValues[2] /= 10;
  lineValues[3] /= 10;

  // Edge Value R
  LOG_INFO("<IR Sensors>\tR on EDGE");
  interface::lcd.setCursor(0, 0);
  interface::lcd.print("IR R on EDGE    ");
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  for (int i = 0; i < 10; i++) {
    edgeValues[1] += analogRead(PIN_IR_FR);
    edgeValues[3] += analogRead(PIN_IR_BR);

    delay(100);
  }
  edgeValues[1] /= 10;
  edgeValues[3] /= 10;

  // Edge Value L
  LOG_INFO("<IR Sensors>\tL on EDGE");
  interface::lcd.setCursor(0, 0);
  interface::lcd.print("IR L on EDGE    ");
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  for (int i = 0; i < 10; i++) {
    edgeValues[0] += analogRead(PIN_IR_FL);
    edgeValues[2] += analogRead(PIN_IR_BL);

    delay(100);
  }
  edgeValues[0] /= 10;
  edgeValues[2] /= 10;

  for (int i = 0; i < 4; i++) {
    LOG_DEBUG("<IR Sensors>\t" + irSensorsNames[i]);
    irSensors[i].setEdgeThreshold((lineValues[i] + edgeValues[i]) / 2);
    irSensors[i].setLineThreshold((tableValues[i] + lineValues[i]) / 2);
  }

  interface::lcd.setCursor(0, 0);
  interface::lcd.print("                ");
}

void hardware::sensors::calibrateLaser() {
  LOG_INFO("<Laser>\t\tCalibrating...");
  laserThreshold = 500;
}

void hardware::sensors::setDefaultThresholds() {
  irSensors[0].setEdgeThreshold(IR_FL_EDGE_THRESHOLD);
  irSensors[0].setLineThreshold(IR_FL_LINE_THRESHOLD);

  irSensors[1].setEdgeThreshold(IR_FR_EDGE_THRESHOLD);
  irSensors[1].setLineThreshold(IR_FR_LINE_THRESHOLD);

  irSensors[2].setEdgeThreshold(IR_BL_EDGE_THRESHOLD);
  irSensors[2].setLineThreshold(IR_BL_LINE_THRESHOLD);

  irSensors[3].setEdgeThreshold(IR_BR_EDGE_THRESHOLD);
  irSensors[3].setLineThreshold(IR_BR_LINE_THRESHOLD);

  laserThreshold = LASER_THRESHOLD;
}

void hardware::sensors::calibrate() {
  LOG_INFO("<Sensors>\tCalibrating...");

  if (bitRead(interface::operationMode, 3)) {
    calibrateIRSensors();
    calibrateLaser();
  } else {
    setDefaultThresholds();
  }
}

void hardware::sensors::loop() {
  irSensors[0].update();
  irSensors[1].update();
  irSensors[2].update();
  irSensors[3].update();

  irDistanceSensors[0].update();
  irDistanceSensors[1].update();

  isBallDetected[0] =
      irDistanceSensors[0].getDistance() < irDistanceSensorThreshold[0];
  isBallDetected[1] =
      irDistanceSensors[1].getDistance() < irDistanceSensorThreshold[1];

  isLaserBlocked = analogRead(PIN_LASER_PHOTORESISTOR) > laserThreshold;
}
