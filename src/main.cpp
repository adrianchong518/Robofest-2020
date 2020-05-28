#include <Arduino.h>
#include <PID.h>

#include "constants.h"
#include "control/manual.h"
#include "hardware/hardware.h"
#include "utils.h"

byte operationMode;

void setup() {
  Serial.begin(115200);

  // Hardware Initialisation
  LOG("<Hardware> Init Start...");
  hardware::init();
  LOG("<Hardware> Init Complete");

  // Read operation mode
  operationMode = hardware::readDIPSwitches();
  LOG("<Hardware> Operatoin Mode: " + String(operationMode, BIN));
  hardware::lcd.print("Mode:" +
                      String(bitRead(operationMode, 0) ? "Manual" : "Auto"));
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Init...     Done");

  // Hardware Calibration
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Calibrate...    ");
  LOG("<Hardware> Calibration Start...");

  hardware::calibrate();

  LOG("<Hardware> Calibration Complete");
  hardware::lcd.setCursor(12, 1);
  hardware::lcd.print("Done");

  // Setting hardware default position (home)
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Homing...       ");

  if (bitRead(operationMode, 1)) {
    LOG("<Hardware> Homing Start...");

    hardware::defaultPosition();

    LOG("<Hardware> Homing Complete");
    hardware::lcd.setCursor(12, 1);
    hardware::lcd.print("Done");
  } else {
    LOG("<Hardware> Homing Skipped");
    hardware::lcd.setCursor(9, 1);
    hardware::lcd.print("Skipped");
  }

  // Start main loop
  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Loop Running... ");
  LOG("Main Loop: Starts...");
}

void loop() {
  hardware::loop();
  if (bitRead(operationMode, 0)) {
    control::manual::loop();
  } else {
  }
}
