#ifndef HARDWARE_HARDWARE_H
#define HARDWARE_HARDWARE_H

#include "hardware/BallHitter.h"
#include "hardware/Mecanum.h"
#include "hardware/Motor.h"
#include "hardware/Rail.h"
#include "hardware/TurnTable.h"
#include "hardware/encoders.h"
#include "hardware/sensors.h"
#include "hardware/servos.h"
#include "hardware/interface.h"

namespace hardware {

extern Rail rail;
extern TurnTable turnTable;

extern Motor hitterMotor;
extern BallHitter ballHitter;

extern Motor wheelFL;
extern Motor wheelFR;
extern Motor wheelBL;
extern Motor wheelBR;
extern Mecanum mecanum;

extern bool isHardwareLoopUpdating;

void init();

void calibrate();

void defaultPosition();

void stopAll();

void loop();

}  // namespace hardware

#endif  // HARDWARE_HARDWARE_H
