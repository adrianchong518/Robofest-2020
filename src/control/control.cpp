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

    hardware::interface::lcd.setCursor(0, 1);
    hardware::interface::lcd.print("Seq Init...     ");
    hardware::interface::lcd.setCursor(0, 0);
    hardware::interface::lcd.print("Set Seq: 0");

    int seqID = 0;
    while (digitalRead(PIN_BUTTON_1)) {
      if (Serial.available()) {
        int inChar = Serial.read();
        if (inChar == '\n') {
          break;
        }

        if (inChar >= '0' && inChar <= '9') {
          seqID = inChar - '0';
          hardware::interface::lcd.setCursor(9, 0);
          hardware::interface::lcd.print(seqID);
        }
      }

      if (digitalRead(PIN_BUTTON_2) == LOW) {
        if (++seqID > 9) {
          seqID = 0;
        }

        hardware::interface::lcd.setCursor(9, 0);
        hardware::interface::lcd.print(seqID);
        delay(200);
      }
    }

    routines::runSeq(seqID);

    hardware::interface::lcd.setCursor(12, 1);
    hardware::interface::lcd.print("Done");

    LOG_INFO("<Control>\tInit Done");
  }

  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
}

void control::loop() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
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
