#include "hardware/sensors/sensors.h"

#include "constants.h"
#include "hardware/interface.h"

hardware::sensors::IRSensor hardware::sensors::IRSensorFL(PIN_IR_FL);
hardware::sensors::IRSensor hardware::sensors::IRSensorFR(PIN_IR_FR);
hardware::sensors::IRSensor hardware::sensors::IRSensorBL(PIN_IR_BL);
hardware::sensors::IRSensor hardware::sensors::IRSensorBR(PIN_IR_BR);

GY53 hardware::sensors::irDistance(&SERIAL_IR_DISTANCE,
                                   SERIAL_IR_DISTANCE_BAUDRATE);

int hardware::sensors::laserThreshold = LASER_THRESHOLD;
bool hardware::sensors::isLaserBlocked = false;

void hardware::sensors::init() {
  LOG_DEBUG("<Sensors>\tInitialising...");

  pinMode(PIN_LASER_PHOTORESISTOR, INPUT);
}

// TODO Find calibration method and process
void hardware::sensors::calibrate() { LOG_INFO("<Sensors>\tCalibrating..."); }

void hardware::sensors::loop() {
  IRSensorFL.update();
  IRSensorFR.update();
  IRSensorBL.update();
  IRSensorBR.update();

  irDistance.update();

  isLaserBlocked = analogRead(PIN_LASER_PHOTORESISTOR) < laserThreshold;
}
