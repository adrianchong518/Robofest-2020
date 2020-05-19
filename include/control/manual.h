#ifndef CONTROL_MANUAL_H
#define CONTROL_MANUAL_H

#include <Arduino.h>

namespace control {
namespace manual {

extern String input;

void loop();
void parseInput();

void hardware(const String &command);

void rail(const String &command);
void turnTable(const String &command);
void ballHitter(const String &command);
void mecanum(const String &command);

void measureDistance(const String &command);

void irSensors(const String &command);
void laserPhotoresistor(const String &command);
void irDistanceSensor(const String &command);

void lcd(const String &command);

}  // namespace manual
}  // namespace control

#endif  // CONTROL_MANUAL_H
