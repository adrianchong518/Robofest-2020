#include <Arduino.h>
#include <PID.h>

#include "constants.h"
#include "hardware/hardware.h"
#include "utils.h"

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
  while (digitalRead(PIN_START_BUTTON))
    ;
  LOG("Calibration Start...");
  hardware::calibrate();
  LOG("Complete");

  // Setting hardware default position (home)
  while (digitalRead(PIN_START_BUTTON))
    ;
  LOG("Setting to default positions...");
  hardware::defaultPosition();
  LOG("Complete");

  while (digitalRead(PIN_START_BUTTON))
    ;
  LOG("Main loop starts...");
}

void loop() { hardware::loop(); }
