#ifndef CONTROL_ROUTINE_H
#define CONTROL_ROUTINE_H

#include <Arduino.h>

namespace control {
namespace routines {

enum RoutineID { NONE, TEST1, TEST2 };

struct Routine {
  RoutineID id = RoutineID::NONE;
  String name = "";

  virtual void init() {}
  virtual bool loop() { return false; }
};

const RoutineID getRoutineIDByName(String name);

struct Test1 : Routine {
  RoutineID id = RoutineID::TEST1;
  String name = "test1";

  int counter = 0;

  void init();
  bool loop();
};

struct Test2 : Routine {
  RoutineID id = RoutineID::TEST2;
  String name = "test2";

  int counter = 0;

  void init();
  bool loop();
};

Routine *const routineList[] = {new Routine, new Test1, new Test2};

}  // namespace routines
}  // namespace control

#endif  // CONTROL_ROUTINE_H