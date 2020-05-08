#ifndef HARDWARE_SENSORS_H
#define HARDWARE_SENSORS_H

#include <GY53.h>

namespace hardware {
namespace sensors {

extern GY53 irDistance;

extern int irThreshold;
extern int laserPOTThreshold;

void init();

void calibrate();

void loop();

bool isBlackDetected(const uint8_t pin_ir);

bool isLaserBlocked();

}  // namespace sensors
}  // namespace hardware

#endif
