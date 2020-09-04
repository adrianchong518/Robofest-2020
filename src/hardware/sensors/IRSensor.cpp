#include "hardware/sensors/IRSensor.h"

#include "hardware/interface.h"
#include "constants.h"

hardware::sensors::IRSensor::IRSensor(const uint8_t pin,
                                      const int deadzoneWidth)
    : m_pin(pin), m_deadzoneWidth(deadzoneWidth) {
  pinMode(m_pin, INPUT);
}

void hardware::sensors::IRSensor::update() {
  m_value = analogRead(m_pin);

  if (m_value < m_lineThreshold - m_deadzoneWidth / 2) {
    m_isLineDetected = true;
  } else if (m_value > m_lineThreshold + m_deadzoneWidth / 2) {
    m_isLineDetected = false;
  }

  if (m_value < m_edgeThreshold - m_deadzoneWidth / 2) {
    m_isEdgeDetected = true;
  } else if (m_value > m_edgeThreshold + m_deadzoneWidth / 2) {
    m_isEdgeDetected = false;
  }
}

int hardware::sensors::IRSensor::getLineThreshold() { return m_lineThreshold; }

void hardware::sensors::IRSensor::setLineThreshold(const int lineThreshold) {
  m_lineThreshold = constrain(lineThreshold, 0, 1023);

  LOG_DEBUG("<IR Sensor>\tLine Threshold Set (" + String(m_lineThreshold) +
            ")");
}

int hardware::sensors::IRSensor::getEdgeThreshold() { return m_edgeThreshold; }

void hardware::sensors::IRSensor::setEdgeThreshold(const int edgeThreshold) {
  m_edgeThreshold = constrain(edgeThreshold, 0, 1023);

  LOG_DEBUG("<IR Sensor>\tEdge Threshold Set (" + String(m_edgeThreshold) +
            ")");
}

int hardware::sensors::IRSensor::getValue() { return m_value; }

bool hardware::sensors::IRSensor::isLineDetected() { return m_isLineDetected; }

bool hardware::sensors::IRSensor::isEdgeDetected() { return m_isEdgeDetected; }