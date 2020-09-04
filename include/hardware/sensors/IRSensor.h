#ifndef HARDWARE_SENSORS_IRSENSOR_H
#define HAREWARE_SENSORS_IRSENSOR_H

#include <Arduino.h>

namespace hardware {
namespace sensors {

class IRSensor {
 private:
  uint8_t m_pin;

  int m_value;

  int m_deadzoneWidth;
  int m_lineThreshold;
  int m_edgeThreshold;

  bool m_isLineDetected;
  bool m_isEdgeDetected;

 public:
  IRSensor(const uint8_t pin, const int deadzoneWidth = 0);

  int getLineThreshold();
  void setLineThreshold(const int lineThreshold);

  int getEdgeThreshold();
  void setEdgeThreshold(const int edgeThreshold);

  void update();

  int getValue();
  bool isLineDetected();
  bool isEdgeDetected();
};

}  // namespace sensors
}  // namespace hardware

#endif  // HAREWARE_SENSORS_IRSENSOR_H
