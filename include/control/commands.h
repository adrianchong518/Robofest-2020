#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include <Arduino.h>

namespace control {
namespace commands {

int parseInput(const String &command);

}  // namespace commands
}  // namespace control

#endif  // CONTROL_COMMANDS_H
