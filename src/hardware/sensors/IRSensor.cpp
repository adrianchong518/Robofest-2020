#include "hardware/sensors/IRSensor.h"

#include "hardware/interface.h"
#include "constants.h"

int hardware::sensors::IRSensor::sm_lineThreshold = IR_LINE_THRESHOLD;
int hardware::sensors::IRSensor::sm_edgeThreshold = IR_EDGE_THRESHOLD;

hardware::sensors::IRSensor::IRSensor(const uint8_t pin) : m_pin(pin) {
  pinMode(m_pin, INPUT);
}

int hardware::sensors::IRSensor::getLineThreshold() { return sm_lineThreshold; }

void hardware::sensors::IRSensor::setLineThreshold(const int lineThreshold) {
  sm_lineThreshold = constrain(lineThreshold, 0, 1023);

  LOG_DEBUG("<IR Sensor>\tLine Threshold Set (" + String(sm_lineThreshold) +
            ")");
}

int hardware::sensors::IRSensor::getEdgeThreshold() { return sm_edgeThreshold; }

void hardware::sensors::IRSensor::setEdgeThreshold(const int edgeThreshold) {
  sm_edgeThreshold = constrain(edgeThreshold, 0, 1023);

  LOG_DEBUG("<IR Sensor>\tEdge Threshold Set (" + String(sm_edgeThreshold) +
            ")");
}

void hardware::sensors::IRSensor::update() {
  m_value = analogRead(m_pin);
  m_isLineDetected = m_value < sm_lineThreshold;
  m_isEdgeDetected = m_value < sm_edgeThreshold;
}

int hardware::sensors::IRSensor::getValue() { return m_value; }

bool hardware::sensors::IRSensor::isLineDetected() { return m_isLineDetected; }

bool hardware::sensors::IRSensor::isEdgeDetected() { return m_isEdgeDetected; }