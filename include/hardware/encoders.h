#ifndef HARDWARE_ENCODERS_H
#define HARDWARE_ENCODERS_H

#include <stdint.h>

namespace hardware {
namespace encoders {

extern int16_t hitterEncoderLocation;
extern uint16_t measureEncoderLocation;

void init();
void defaultPosition();
void loop();

uint16_t readLocation(uint8_t pin_encoderOE);
void resetLocation(uint8_t pin_encoderRST);

}  // namespace encoders
}  // namespace hardware

#endif  // HARDWARE_ENCODERS_H
