#ifndef CONTROL_MANUAL_H
#define CONTROL_MANUAL_H

#include <Arduino.h>

namespace control {
namespace manual {

extern String input;

void loop();
void parseInput();

void rail(double target);
void turnTable(double target);
void ballHitter(double target);

}  // namespace manual
}  // namespace control

#endif  // CONTROL_MANUAL_H
