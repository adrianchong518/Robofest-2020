#ifndef HARDWARE_SERVOS_H
#define HARDWARE_SERVOS_H

#include <Servo.h>

namespace hardware {
namespace servos {

extern Servo guideLeft;
extern Servo guideRight;
extern Servo holderLeft;
extern Servo holderRight;
extern Servo measureServo;

extern bool isGuideLeftExtended;
extern bool isGuideRightExtended;
extern bool isHolderLeftExtended;
extern bool isHolderRightExtended;
extern bool isMeasureServoExtended;

void init();
void defaultPosition();

void setGuideLeft(bool isExtended);
void setGuideRight(bool isExtended);
void setHolderLeft(bool isExtended);
void setHolderRight(bool isExtended);
void setMeasureServo(bool isExtended);

}  // namespace servos
}  // namespace hardware

#endif  // HARDWARE_SERVOS_H
