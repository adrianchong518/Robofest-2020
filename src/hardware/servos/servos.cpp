#include "hardware/servos/servos.h"

#include "constants.h"

typedef hardware::servos::CustomServo CustomServo;

CustomServo hardware::servos::guideLeft(PIN_GUIDE_LEFT, GUIDE_MS_PER_DEG);
CustomServo hardware::servos::guideRight(PIN_GUIDE_RIGHT, GUIDE_MS_PER_DEG);
CustomServo hardware::servos::holderLeft(PIN_HOLDER_LEFT, HOLDER_MS_PER_DEG);
CustomServo hardware::servos::holderRight(PIN_HOLDER_RIGHT, HOLDER_MS_PER_DEG);
CustomServo hardware::servos::measureServo(PIN_MEASURE_SERVO,
                                           MEASURE_SERVO_MS_PER_DEG);

bool hardware::servos::isGuideLeftExtented = false;
bool hardware::servos::isGuideRightExtented = false;
bool hardware::servos::isHolderLeftExtented = false;
bool hardware::servos::isHolderRightExtented = false;
bool hardware::servos::isMeasureServoExtented = false;

void hardware::servos::defaultPosition() {
  setGuideLeft(false);
  setGuideRight(false);
  setHolderLeft(false);
  setHolderRight(false);
  setMeasureServo(false);
}

void hardware::servos::loop() {
  guideLeft.update();
  guideRight.update();
  holderLeft.update();
  holderRight.update();
  measureServo.update();
}

void hardware::servos::setGuideLeft(bool isExtended) {
  if (isExtended) {
    guideLeft.setTarget(GUIDE_LEFT_EXTENDED_POS);
  } else {
    guideLeft.setTarget(GUIDE_LEFT_RETRACTED_POS);
  }
  isGuideLeftExtented = isExtended;
}

void hardware::servos::setGuideRight(bool isExtended) {
  if (isExtended) {
    guideRight.setTarget(GUIDE_RIGHT_EXTENDED_POS);
  } else {
    guideRight.setTarget(GUIDE_RIGHT_RETRACTED_POS);
  }
  isGuideRightExtented = isExtended;
}

void hardware::servos::setHolderLeft(bool isExtended) {
  if (isExtended) {
    holderLeft.setTarget(HOLDER_LEFT_EXTENDED_POS);
  } else {
    holderLeft.setTarget(HOLDER_LEFT_RETRACTED_POS);
  }
  isHolderLeftExtented = isExtended;
}

void hardware::servos::setHolderRight(bool isExtended) {
  if (isExtended) {
    holderRight.setTarget(HOLDER_RIGHT_EXTENDED_POS);
  } else {
    holderRight.setTarget(HOLDER_RIGHT_RETRACTED_POS);
  }
  isHolderRightExtented = isExtended;
}

void hardware::servos::setMeasureServo(bool isExtended) {
  if (isExtended) {
    measureServo.setTarget(MEASURE_SERVO_EXTENDED_POS);
  } else {
    measureServo.setTarget(MEASURE_SERVO_RETRACTED_POS);
  }
  isMeasureServoExtented = isExtended;
}
