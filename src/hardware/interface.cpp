#include "hardware/interface.h"

#include "constants.h"

hd44780_I2Cexp hardware::interface::lcd;

byte hardware::interface::operationMode = 0;

void hardware::interface::init() {
  LOG_DEBUG("<Interface>\tInitialising...");
  int lcdBeginStatus = lcd.begin(LCD_NUM_COLS, LCD_NUM_ROWS);
  if (lcdBeginStatus) {
    hd44780::fatalError(lcdBeginStatus);
  }
  lcd.clear();

  pinMode(PIN_BUZZER, OUTPUT);

  DDR_SW_BTN = 0x00 | (DDR_SW_BTN & 0x01);
  PORT_SW_BTN = 0x00 | (PORT_SW_BTN & 0x01);

  operationMode = readDIPSwitches();
  LOG_INFO("<Interface>\tOperation Mode: " + String(operationMode, BIN));
}

byte hardware::interface::readDIPSwitches() {
  return (~PIN_SW_BTN >> BITS_DIP_SW) & 0x0F;
}