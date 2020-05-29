#include "hardware/encoders.h"

#include <Arduino.h>

#include "constants.h"

int16_t hardware::encoders::hitterEncoderLocation = 0;
uint16_t hardware::encoders::measureEncoderLocation = 0;

void hardware::encoders::init() {
  // Pins
  pinMode(PIN_ENCODER_SEL, OUTPUT);
  digitalWrite(PIN_ENCODER_SEL, LOW);

  pinMode(PIN_HITTER_ENCODER_OE, OUTPUT);
  digitalWrite(PIN_HITTER_ENCODER_OE, HIGH);

  pinMode(PIN_HITTER_ENCODER_RST, OUTPUT);
  digitalWrite(PIN_HITTER_ENCODER_RST, HIGH);

  pinMode(PIN_MEASURE_ENCODER_OE, OUTPUT);
  digitalWrite(PIN_MEASURE_ENCODER_OE, HIGH);

  pinMode(PIN_MEASURE_ENCODER_RST, OUTPUT);
  digitalWrite(PIN_MEASURE_ENCODER_RST, HIGH);

  // Generate clock output
  pinMode(PIN_ENCODER_CLK, OUTPUT);
  TCCR2A = bit(WGM21) | bit(COM2A0);
  TCCR2B = bit(CS20);
  OCR2A = 0;

  // Set up PORT
  DDR_ENCODER_BUS = 0x00;
  PORT_ENCODER_BUS = 0x00;
}

void hardware::encoders::defaultPosition() {
  resetLocation(PIN_HITTER_ENCODER_RST);
  resetLocation(PIN_MEASURE_ENCODER_RST);
}

void hardware::encoders::loop() {
  hitterEncoderLocation = (int16_t)readLocation(PIN_HITTER_ENCODER_OE);
  measureEncoderLocation = readLocation(PIN_MEASURE_ENCODER_OE);
}

uint16_t hardware::encoders::readLocation(uint8_t pin_encoderOE) {
  uint16_t location;

  digitalWrite(pin_encoderOE, LOW);

  digitalWrite(PIN_ENCODER_SEL, LOW);
  location = PIN_ENCODER_BUS << 8;
  digitalWrite(PIN_ENCODER_SEL, HIGH);
  location |= PIN_ENCODER_BUS;

  digitalWrite(pin_encoderOE, HIGH);

  return location;
}

void hardware::encoders::resetLocation(uint8_t pin_encoderRST) {
  digitalWrite(pin_encoderRST, LOW);
  delay(1);
  digitalWrite(pin_encoderRST, HIGH);
}
