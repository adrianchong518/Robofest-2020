#include "hardware/servos.h"

#include "constants.h"

Servo hardware::servos::guideLeft;
Servo hardware::servos::guideRight;
Servo hardware::servos::holderLeft;
Servo hardware::servos::holderRight;
Servo hardware::servos::measureServo;

bool hardware::servos::isGuideLeftExtented = false;
bool hardware::servos::isGuideRightExtented = false;
bool hardware::servos::isHolderLeftExtented = false;
bool hardware::servos::isHolderRightExtented = false;
bool hardware::servos::isMeasureServoExtented = false;

void hardware::servos::init() {
  guideLeft.attach(PIN_GUIDE_LEFT);
  guideRight.attach(PIN_GUIDE_RIGHT);
  holderLeft.attach(PIN_HOLDER_LEFT);
  holderRight.attach(PIN_HOLDER_RIGHT);
  measureServo.attach(PIN_MEASURE_SERVO);
}

void hardware::servos::defaultPosition() {
  setGuideLeft(false);
  setGuideRight(false);
  setHolderLeft(false);
  setHolderRight(false);
  setMeasureServo(false);
}

void hardware::servos::setGuideLeft(bool isExtended) {
  if (isExtended) {
    guideLeft.write(GUIDE_LEFT_EXTENDED_POS);
  } else {
    guideLeft.write(GUIDE_LEFT_RETRACTED_POS);
  }
  isGuideLeftExtented = isExtended;
}

void hardware::servos::setGuideRight(bool isExtended) {
  if (isExtended) {
    guideRight.write(GUIDE_RIGHT_EXTENDED_POS);
  } else {
    guideRight.write(GUIDE_RIGHT_RETRACTED_POS);
  }
  isGuideRightExtented = isExtended;
}

void hardware::servos::setHolderLeft(bool isExtended) {
  if (isExtended) {
    holderLeft.write(HOLDER_LEFT_EXTENDED_POS);
  } else {
    holderLeft.write(HOLDER_LEFT_RETRACTED_POS);
  }
  isHolderLeftExtented = isExtended;
}

void hardware::servos::setHolderRight(bool isExtended) {
  if (isExtended) {
    holderRight.write(HOLDER_RIGHT_EXTENDED_POS);
  } else {
    holderRight.write(HOLDER_RIGHT_RETRACTED_POS);
  }
  isHolderRightExtented = isExtended;
}

void hardware::servos::setMeasureServo(bool isExtended) {
  if (isExtended) {
    measureServo.write(MEASURE_EXTENDED_POS);
  } else {
    measureServo.write(MEASURE_RETRACTED_POS);
  }
  isMeasureServoExtented = isExtended;
}
