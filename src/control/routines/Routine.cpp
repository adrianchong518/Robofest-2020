#include "control/routines/Routine.h"

#include "hardware/hardware.h"

const control::routines::RoutineID control::routines::getRoutineIDByName(
    String name) {
  for (Routine *const routine : routineList) {
    if (routine->name == name) {
      return routine->id;
    }
  }

  return RoutineID::NONE;
}

void control::routines::Test1::init() {
  LOG_DEBUG("<R/Test1>\tInit");
  counter = 0;
}

bool control::routines::Test1::loop() {
  LOG_DEBUG("<R/Test1>\tLoop" + String(++counter));

  if (counter == 2) {
    return true;
  }
  return false;
}

void control::routines::Test2::init() {
  LOG_DEBUG("<R/Test2>\tInit");
  counter = 0;
}

bool control::routines::Test2::loop() {
  LOG_DEBUG("<R/Test2>\tLoop" + String(++counter));

  if (counter == 5) {
    return true;
  }
  return false;
}