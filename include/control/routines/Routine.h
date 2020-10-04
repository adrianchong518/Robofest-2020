#ifndef CONTROL_ROUTINE_H
#define CONTROL_ROUTINE_H

#include <Arduino.h>

namespace control {
namespace routines {

enum RoutineID {
  NONE,
  LINE_FWD,
  LINE_REVERSE,
  EDGE_BACK,
  EDGE_LEFT,
  EDGE_RIGHT,
  BALL_FWD,
  BALL_SIDE,
  BALL_5,
  HIT_1,
  HIT_2,
  HIT_3,
  HIT_4,
  HIT_5
};

struct {
  bool isBallInHolderL = false;
  bool isBallInHolderR = false;

  byte positionState = 0;
} data;

struct Routine {
  RoutineID id;
  String name;

  Routine(RoutineID id = RoutineID::NONE, const String &name = "")
      : id(id), name(name) {}

  virtual void init() {}
  virtual bool loop() { return false; }
};

const RoutineID getRoutineIDByName(String name);

struct FindLineForward : Routine {
  FindLineForward() : Routine(RoutineID::LINE_FWD, "line-fwd") {}

  void init();
  bool loop();
};

struct FindLineReverse : Routine {
  FindLineReverse() : Routine(RoutineID::LINE_REVERSE, "line-rev") {}

  void init();
  bool loop();
};

struct FindEdgeBackward : Routine {
  FindEdgeBackward() : Routine(RoutineID::EDGE_BACK, "edge-back") {}

  void init();
  bool loop();
};

struct FindEdgeLeft : Routine {
  FindEdgeLeft() : Routine(RoutineID::EDGE_LEFT, "edge-left") {}

  void init();
  bool loop();
};

struct FindEdgeRight : Routine {
  FindEdgeRight() : Routine(RoutineID::EDGE_RIGHT, "edge-right") {}

  void init();
  bool loop();
};

struct FindBallForward : Routine {
  FindBallForward() : Routine(RoutineID::BALL_FWD, "ball-fwd") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
  uint16_t m_measureStart = 0;
};

struct FindBallSideward : Routine {
  FindBallSideward() : Routine(RoutineID::BALL_SIDE, "ball-side") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

struct FindBall5 : Routine {
  FindBall5() : Routine(RoutineID::BALL_5, "ball-5") {}

  void init();
  bool loop();

 private:
  Routine *m_runningSubroutine = nullptr;
  byte m_stage = 0;
  unsigned long timer;
};

struct HitBall1 : Routine {
  HitBall1() : Routine(RoutineID::HIT_1, "hit-1") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

struct HitBall2 : Routine {
  HitBall2() : Routine(RoutineID::HIT_2, "hit-2") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

struct HitBall3 : Routine {
  HitBall3() : Routine(RoutineID::HIT_3, "hit-3") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

struct HitBall4 : Routine {
  HitBall4() : Routine(RoutineID::HIT_4, "hit-4") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

struct HitBall5 : Routine {
  HitBall5() : Routine(RoutineID::HIT_5, "hit-5") {}

  void init();
  bool loop();

 private:
  byte m_stage = 0;
};

Routine *const routineList[] = {
    new Routine,          new FindLineForward,  new FindLineReverse,
    new FindEdgeBackward, new FindEdgeLeft,     new FindEdgeRight,
    new FindBallForward,  new FindBallSideward, new FindBall5,
    new HitBall1,         new HitBall2,         new HitBall3,
    new HitBall4,         new HitBall5};

}  // namespace routines
}  // namespace control

#endif  // CONTROL_ROUTINE_H