#include <Arduino.h>

#include "constants.h"
#include "control/manual.h"
#include "hardware/hardware.h"

void setup() {
  Serial.begin(115200);

  hardware::init();

  hardware::interface::lcd.setCursor(0, 1);
  hardware::interface::lcd.print("Init...     Done");

  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::interface::lcd.setCursor(0, 1);
  hardware::interface::lcd.print("Calibrate...    ");

  hardware::calibrate();

  hardware::interface::lcd.setCursor(12, 1);
  hardware::interface::lcd.print("Done");

  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::interface::lcd.setCursor(0, 1);
  hardware::interface::lcd.print("Homing...       ");
  if (bitRead(hardware::interface::operationMode, 1)) {
    hardware::defaultPosition();

    hardware::interface::lcd.setCursor(12, 1);
    hardware::interface::lcd.print("Done");
  } else {
    LOG_INFO("<Hardware>\tHoming Skipped");
    hardware::interface::lcd.setCursor(9, 1);
    hardware::interface::lcd.print("Skipped");
  }

  while (digitalRead(PIN_BUTTON_1) && Serial.read() != '\n')
    ;
  hardware::interface::lcd.setCursor(0, 1);
  hardware::interface::lcd.print("Loop Running... ");
  LOG_INFO("Main Loop Starts...");
}

void loop() {
  hardware::loop();
  control::manual::loop();
}
