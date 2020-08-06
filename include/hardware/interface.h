#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include <Arduino.h>

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#if DEBUG == 1
#define LOG_DEBUG(msg) Serial.println("[DEBUG]" + String(msg))
#else
#define LOG_DEBUG(msg)
#endif

#define LOG_INFO(msg) Serial.println("[INFO]" + String(msg))
#define LOG_ERROR(msg) Serial.println("[ERROR]" + String(msg))

namespace hardware {
namespace interface {

extern hd44780_I2Cexp lcd;

extern byte operationMode;

void init();

byte readDIPSwitches();

}  // namespace interface
}  // namespace hardware

#endif  // HARDWARE_INTERFACE_H