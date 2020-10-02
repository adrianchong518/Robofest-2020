#ifndef HARDWARE_SENSORS_H
#define HARDWARE_SENSORS_H

#include <GY53.h>

#include "hardware/sensors/IRSensor.h"

namespace hardware {
namespace sensors {

extern IRSensor irSensors[4];
static const String irSensorsNames[4] = {"FL", "FR", "BL", "BR"};

extern GY53 irDistanceSensors[2];
static const String irDistanceSensorNames[2] = {"L", "R"};

extern uint16_t irDistanceSensorThreshold[2];
extern bool isBallDetected[2];

extern int laserThreshold;
extern bool isLaserBlocked;

void init();

void calibrateIRSensors();
void calibrateLaser();

void setDefaultThresholds();
void calibrate();

void loop();

}  // namespace sensors
}  // namespace hardware

#endif
