#ifndef HARDWARE_SENSORS_IRSENSOR_H
#define HAREWARE_SENSORS_IRSENSOR_H

#include <Arduino.h>

namespace hardware {
namespace sensors {

class IRSensor {
 private:
  static int sm_lineThreshold;
  static int sm_edgeThreshold;

  uint8_t m_pin;

  int m_value;
  bool m_isLineDetected;
  bool m_isEdgeDetected;

 public:
  IRSensor(const uint8_t pin);

  static int getLineThreshold();
  static void setLineThreshold(const int lineThreshold);

  static int getEdgeThreshold();
  static void setEdgeThreshold(const int edgeThreshold);

  void update();

  int getValue();
  bool isLineDetected();
  bool isEdgeDetected();
};

}  // namespace sensors
}  // namespace hardware

#endif  // HAREWARE_SENSORS_IRSENSOR_H
