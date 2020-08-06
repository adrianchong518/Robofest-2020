#include "hardware/servos.h"

#include "constants.h"
#include "hardware/interface.h"

Servo hardware::servos::guideLeft;
Servo hardware::servos::guideRight;
Servo hardware::servos::holderLeft;
Servo hardware::servos::holderRight;
Servo hardware::servos::measureServo;

bool hardware::servos::isGuideLeftExtended = false;
bool hardware::servos::isGuideRightExtended = false;
bool hardware::servos::isHolderLeftExtended = false;
bool hardware::servos::isHolderRightExtended = false;
bool hardware::servos::isMeasureServoExtended = false;

void hardware::servos::init() {
  LOG_DEBUG("<Servos> Initialising...");

  guideLeft.attach(PIN_GUIDE_LEFT);
  guideRight.attach(PIN_GUIDE_RIGHT);
  holderLeft.attach(PIN_HOLDER_LEFT);
  holderRight.attach(PIN_HOLDER_RIGHT);
  measureServo.attach(PIN_MEASURE_SERVO);
}

void hardware::servos::defaultPosition() {
  LOG_INFO("<Servos> Setting default positions");

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
  isGuideLeftExtended = isExtended;
  LOG_DEBUG("<Servos> Guide Left " + isGuideLeftExtended ? "Extended"
                                                         : "Retracted");
}

void hardware::servos::setGuideRight(bool isExtended) {
  if (isExtended) {
    guideRight.write(GUIDE_RIGHT_EXTENDED_POS);
  } else {
    guideRight.write(GUIDE_RIGHT_RETRACTED_POS);
  }
  isGuideRightExtended = isExtended;
  LOG_DEBUG("<Servos> Guide Right " + isGuideRightExtended ? "Extended"
                                                           : "Retracted");
}

void hardware::servos::setHolderLeft(bool isExtended) {
  if (isExtended) {
    holderLeft.write(HOLDER_LEFT_EXTENDED_POS);
  } else {
    holderLeft.write(HOLDER_LEFT_RETRACTED_POS);
  }
  isHolderLeftExtended = isExtended;
  LOG_DEBUG("<Servos> Holder Left " + isHolderLeftExtended ? "Extended"
                                                           : "Retracted");
}

void hardware::servos::setHolderRight(bool isExtended) {
  if (isExtended) {
    holderRight.write(HOLDER_RIGHT_EXTENDED_POS);
  } else {
    holderRight.write(HOLDER_RIGHT_RETRACTED_POS);
  }
  isHolderRightExtended = isExtended;
  LOG_DEBUG("<Servos> Holder Right " + isHolderRightExtended ? "Extended"
                                                             : "Retracted");
}

void hardware::servos::setMeasureServo(bool isExtended) {
  if (isExtended) {
    measureServo.write(MEASURE_EXTENDED_POS);
  } else {
    measureServo.write(MEASURE_RETRACTED_POS);
  }
  isMeasureServoExtended = isExtended;
  LOG_DEBUG("<Servos> Measure Servo " + isMeasureServoExtended ? "Extended"
                                                               : "Retracted");
}
