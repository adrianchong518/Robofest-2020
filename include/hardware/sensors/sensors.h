#ifndef HARDWARE_SENSORS_H
#define HARDWARE_SENSORS_H

#include <GY53.h>

#include "hardware/sensors/IRSensor.h"

namespace hardware {
namespace sensors {

extern IRSensor IRSensorFL;
extern IRSensor IRSensorFR;
extern IRSensor IRSensorBL;
extern IRSensor IRSensorBR;

extern GY53 irDistance;

extern int laserThreshold;
extern bool isLaserBlocked;

void init();

void calibrate();

void loop();

}  // namespace sensors
}  // namespace hardware

#endif
