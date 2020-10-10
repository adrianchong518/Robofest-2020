#ifndef CONTROL_ROUTINES_H
#define CONTROL_ROUTINES_H

#include <Arduino.h>

#include "control/routines/Routine.h"

namespace control {
namespace routines {

const RoutineID seq0[] = {BALL_5,   BALL_FWD,  HIT_4, BALL_FWD,     HIT_1,
                          HIT_5,    BALL_SIDE, HIT_2, LINE_REVERSE, EDGE_RIGHT,
                          BALL_FWD, HIT_3,     NONE};
const RoutineID seq1[] = {
    BALL_5,    BALL_FWD, HIT_4,        EDGE_RIGHT, BALL_FWD, HIT_3, HIT_5,
    BALL_SIDE, HIT_2,    LINE_REVERSE, EDGE_LEFT,  BALL_FWD, HIT_1, NONE};
const RoutineID *const seqList[10] = {seq0,    seq1,    nullptr, nullptr,
                                      nullptr, nullptr, nullptr, nullptr,
                                      nullptr, nullptr};

extern RoutineID runningRoutine;
extern const RoutineID *runningSeqPtr;

void loop();

void runRoutine(const RoutineID &routineID);
void runSeq(const int seqID);

}  // namespace routines
}  // namespace control

#endif  // CONTROL_ROUTINES_H