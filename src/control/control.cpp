#include "control/control.h"

#include "hardware/hardware.h"
#include "control/commands.h"
#include "control/routines/routines.h"

String control::input = "";

void control::init() {
  if (bitRead(hardware::interface::operationMode, 0)) {
    LOG_INFO("<Control>\tInit Skipped");
    hardware::interface::lcd.setCursor(0, 1);
    hardware::interface::lcd.print("Seq Init Skipped");
  } else {
    LOG_INFO("<Control>\tInit Start...");

    int seqId = bitRead(hardware::interface::operationMode, 3);
    routines::runSeq(seqId);

    LOG_INFO("<Control>\tInit Done");
  }
}

void control::loop() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      input.trim();
      if (commands::parseInput(input) == -1) {
        LOG_ERROR("Invalid Input: " + input);
      }
      input = "";
    } else if (inChar == '\b') {
      input.remove(input.length() - 1);
    } else if (inChar == 'x') {
      hardware::stopAll();
      routines::runRoutine(routines::RoutineID::NONE);
    } else {
      input.concat(inChar);
    }
  }

  routines::loop();
}
