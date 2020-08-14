#include "control/routines/routines.h"

#include "hardware/interface.h"

control::routines::RoutineID control::routines::runningRoutine =
    control::routines::RoutineID::NONE;

const control::routines::RoutineID *control::routines::runningSeqPtr = nullptr;

void control::routines::loop() {
  if (runningSeqPtr != nullptr) {
    if (runningRoutine == RoutineID::NONE) {
      runRoutine(*(++runningSeqPtr));

      if (runningRoutine == RoutineID::NONE) {
        runningSeqPtr = nullptr;
      }
    }
  }

  if (routineList[runningRoutine]->loop()) {
    runningRoutine = RoutineID::NONE;
  }
}

void control::routines::runRoutine(
    const control::routines::RoutineID &routineID) {
  LOG_DEBUG("<Routines>\tRunning Routine: " + String(routineID));

  runningRoutine = routineID;
  routineList[runningRoutine]->init();
}

void control::routines::runSeq(const int seqID) {
  if (seqID < 0 || seqID > 9 || seqList[seqID] == nullptr) {
    LOG_ERROR("<Routines>\tSequence " + String(seqID) + " Not Valid");
    return;
  }

  LOG_DEBUG("<Routines>\tRunning Sequence " + String(seqID));

  runningSeqPtr = seqList[seqID];
  runRoutine(*runningSeqPtr);
}