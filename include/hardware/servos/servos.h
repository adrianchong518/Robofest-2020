#ifndef HARDWARE_SERVOS_H
#define HARDWARE_SERVOS_H

#include <hardware/servos/CustomServo.h>

namespace hardware {
namespace servos {

extern CustomServo guideLeft;
extern CustomServo guideRight;
extern CustomServo holderLeft;
extern CustomServo holderRight;
extern CustomServo measureServo;

extern bool isGuideLeftExtented;
extern bool isGuideRightExtented;
extern bool isHolderLeftExtented;
extern bool isHolderRightExtented;
extern bool isMeasureServoExtented;

void defaultPosition();
void loop();

void setGuideLeft(bool isExtended);
void setGuideRight(bool isExtended);
void setHolderLeft(bool isExtended);
void setHolderRight(bool isExtended);
void setMeasureServo(bool isExtended);

}  // namespace servos
}  // namespace hardware

#endif  // HARDWARE_SERVOS_H
