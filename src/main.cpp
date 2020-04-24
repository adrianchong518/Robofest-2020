#include <Arduino.h>
#include <PID.h>

#include "constants.h"
#include "control/manual.h"
#include "hardware/hardware.h"
#include "utils.h"

byte operationMode;

void setup() {
  Serial.begin(115200);

  // Disable Interrupts
  noInterrupts();

  // Hardware Initialisation
  LOG("Initialisation Start...");
  hardware::init();
  LOG("Complete");

  // Reenable Interrupts
  interrupts();

  // Hardware Calibration
  while (digitalRead(PIN_BUTTON_1))
    ;
  LOG("Calibration Start...");
  hardware::calibrate();
  LOG("Complete");

  // Setting hardware default position (home)
  while (digitalRead(PIN_BUTTON_1))
    ;
  LOG("Setting to default positions...");
  hardware::defaultPosition();
  LOG("Complete");

  operationMode = hardware::readDIPSwitches();
  while (digitalRead(PIN_BUTTON_1))
    ;
  LOG("Main loop starts...");
}

void loop() {
  hardware::loop();
  if (bitRead(operationMode, 0)) {
    control::manual::loop();
  } else {
  }
}
