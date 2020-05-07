#ifndef HARDWARE_SENSORS_H
#define HARDWARE_SENSORS_H

#include <GY53.h>

namespace hardware {
namespace sensors {

extern GY53 irDistance;

extern int irFLThreshold;
extern int irFRThreshold;
extern int irBLThreshold;
extern int irBRThreshold;

extern int laserPOTThreshold;

void init();

void calibrate();

void loop();

bool isBlackDetectedFL();
bool isBlackDetectedFR();
bool isBlackDetectedBL();
bool isBlackDetectedBR();

bool isLaserBlocked();

}  // namespace sensors
}  // namespace hardware

#endif
