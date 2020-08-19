#include "hardware/sensors.h"

#include "constants.h"
#include "hardware/interface.h"

GY53 hardware::sensors::irDistance(&SERIAL_IR_DISTANCE,
                                   SERIAL_IR_DISTANCE_BAUDRATE);

int hardware::sensors::irLineThreshold = 0;
int hardware::sensors::irEdgeThreshold = 0;
int hardware::sensors::photoresistorThreshold = 0;

void hardware::sensors::init() {
  LOG_DEBUG("<Sensors>\tInitialising...");

  pinMode(PIN_IR_FL, INPUT);
  pinMode(PIN_IR_FR, INPUT);
  pinMode(PIN_IR_BL, INPUT);
  pinMode(PIN_IR_BR, INPUT);

  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);
}

// TODO Find calibration method and process
void hardware::sensors::calibrate() { LOG_INFO("<Sensors>\tCalibrating..."); }

void hardware::sensors::loop() { irDistance.update(); }

bool hardware::sensors::isLineDetected(const uint8_t pin_ir) {
  return analogRead(pin_ir) < irLineThreshold;
}

bool hardware::sensors::isEdgeDetected(const uint8_t pin_ir) {
  return analogRead(pin_ir) < irEdgeThreshold;
}

bool hardware::sensors::isLaserBlocked() {
  return analogRead(PIN_LASER_PHOTORESISTOR) < photoresistorThreshold;
}
