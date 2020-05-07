#include "hardware/sensors.h"

#include "constants.h"

GY53 hardware::sensors::irDistance(&SERIAL_IR_DISTANCE,
                                   SERIAL_IR_DISTANCE_BAUDRATE);

int hardware::sensors::irFLThreshold = 0;
int hardware::sensors::irFRThreshold = 0;
int hardware::sensors::irBLThreshold = 0;
int hardware::sensors::irBRThreshold = 0;

int hardware::sensors::laserPOTThreshold = 0;

void hardware::sensors::init() {
  // IR Sensors
  pinMode(PIN_IR_FL, INPUT);
  pinMode(PIN_IR_FR, INPUT);
  pinMode(PIN_IR_BL, INPUT);
  pinMode(PIN_IR_BR, INPUT);

  // Laser Photoresistor
  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);
}

// TODO Find calibration method and process
void hardware::sensors::calibrate() {}

void hardware::sensors::loop() { irDistance.update(); }

bool hardware::sensors::isBlackDetectedFL() {
  return analogRead(PIN_IR_FL) < irFLThreshold;
}

bool hardware::sensors::isBlackDetectedFR() {
  return analogRead(PIN_IR_FR) < irFRThreshold;
}

bool hardware::sensors::isBlackDetectedBL() {
  return analogRead(PIN_IR_BL) < irBLThreshold;
}

bool hardware::sensors::isBlackDetectedBR() {
  return analogRead(PIN_IR_BR) < irBRThreshold;
}

bool hardware::sensors::isLaserBlocked() {
  return analogRead(PIN_LASER_PHOTORESISTOR) < laserPOTThreshold;
}
