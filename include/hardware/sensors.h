#ifndef HARDWARE_SENSORS_H
#define HARDWARE_SENSORS_H

#include <GY53.h>

namespace hardware {
namespace sensors {

extern GY53 irDistance;

extern int irLineThreshold;
extern int irEdgeThreshold;
extern int photoresistorThreshold;

void init();

void calibrate();

void loop();

bool isLineDetected(const uint8_t pin_ir);
bool isEdgeDetected(const uint8_t pin_ir);

bool isLaserBlocked();

}  // namespace sensors
}  // namespace hardware

#endif
