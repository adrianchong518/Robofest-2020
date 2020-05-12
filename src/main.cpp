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
  LOG("Initialisation Start...");
  hardware::init();
  LOG("Complete");

  // Read operation mode
  operationMode = hardware::readDIPSwitches();
  hardware::lcd.print("Mode:" +
                      String(bitRead(operationMode, 0) ? "Manual" : "Auto"));
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Init...     Done");

  // Hardware Calibration
  while (digitalRead(PIN_BUTTON_1))
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Calibrate...    ");
  LOG("Calibration Start...");

  hardware::calibrate();

  LOG("Complete");
  hardware::lcd.setCursor(12, 1);
  hardware::lcd.print("Done");

  // Setting hardware default position (home)
  while (digitalRead(PIN_BUTTON_1))
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Homing...       ");
  LOG("Setting to default positions...");

  hardware::defaultPosition();

  LOG("Complete");
  hardware::lcd.setCursor(12, 1);
  hardware::lcd.print("Done");

  // Start main loop
  while (digitalRead(PIN_BUTTON_1))
    ;
  hardware::lcd.setCursor(0, 1);
  hardware::lcd.print("Loop Running... ");
  LOG("Main loop starts...");
}

void loop() {
  hardware::loop();
  if (bitRead(operationMode, 0)) {
    control::manual::loop();
  } else {
  }
}
