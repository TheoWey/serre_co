#ifndef SENSOR_HH
#define SENSOR_HH

// Includes
#include "../../../Core/Inc/adc.h"
#include <cstdint>
/**
 * @namespace sensor
 * @brief Contains classes and methods for handling various sensors.
 */

namespace sensor {

/**
 * @class Sensor
 * @brief Abstract base class for sensor management.
 *
 * This class provides an interface for initializing, reading, and processing
 * data from sensors. It also includes a private configuration method and
 * allows for timeout configuration.
 *
 * @note Derived classes must implement the virtual methods to provide specific
 *       sensor functionality.
 */
class Sensor {
  public:
    virtual ~Sensor();

    virtual HAL_StatusTypeDef readData() = 0;
    virtual void processData() = 0;

  protected:
    HAL_StatusTypeDef sensor_readHelper(uint32_t *outValue, uint32_t timeout);
    ADC_HandleTypeDef *m_channel; ///< Pointer to the ADC channel configuration
};

} // namespace sensor

#endif // SENSOR_HH
