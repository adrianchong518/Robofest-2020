#include "hardware/sensors.h"

#include "constants.h"
#include "hardware/interface.h"

GY53 hardware::sensors::irDistance(&SERIAL_IR_DISTANCE,
                                   SERIAL_IR_DISTANCE_BAUDRATE);

int hardware::sensors::irThreshold = 0;
int hardware::sensors::laserPOTThreshold = 0;

void hardware::sensors::init() {
  LOG_DEBUG("<Sensors> Initialising...");

  pinMode(PIN_IR_FL, INPUT);
  pinMode(PIN_IR_FR, INPUT);
  pinMode(PIN_IR_BL, INPUT);
  pinMode(PIN_IR_BR, INPUT);

  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);
}

// TODO Find calibration method and process
void hardware::sensors::calibrate() { LOG_INFO("<Sensors> Calibrating..."); }

void hardware::sensors::loop() { irDistance.update(); }

bool hardware::sensors::isBlackDetected(const uint8_t pin_ir) {
  return analogRead(pin_ir) < irThreshold;
}

bool hardware::sensors::isLaserBlocked() {
  return analogRead(PIN_LASER_PHOTORESISTOR) < laserPOTThreshold;
}
