#ifndef HARDWARE_HARDWARE_H
#define HARDWARE_HARDWARE_H

#include <GY53.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include "constants.h"
#include "hardware/Mecanum.h"
#include "hardware/Motor.h"
#include "hardware/Rail.h"
#include "hardware/TurnTable.h"
#include "hardware/encoders.h"
#include "hardware/servos.h"

namespace hardware {

extern Rail rail;
extern TurnTable turnTable;

extern Motor hitterMotor;

extern Motor wheelFL;
extern Motor wheelFR;
extern Motor wheelBL;
extern Motor wheelBR;
extern Mecanum mecanum;

extern PID hitterPID;

extern GY53 irDistance;

extern hd44780_I2Cexp lcd;

void init();

void calibrate();

void defaultPosition();

void loop();

}  // namespace hardware

#endif  // HARDWARE_HARDWARE_H
