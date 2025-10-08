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
    void processData();

  protected:
    HAL_StatusTypeDef sensor_readHelper(uint32_t *outValue, uint32_t timeout);
    ADC_HandleTypeDef
        *m_adcHandle; ///< Pointer to the ADC channel configuration

    bool m_dataValid = false;    ///< Flag indicating if the data is valid.
    uint16_t m_rawADC[10] = {0}; ///< Raw ADC value from the sensor.
    float m_processedValue = 0;  ///< Processed sensor value.
    uint8_t m_sampleIndex = 0;   ///< Index for sampling multiple readings.
    uint32_t m_adcTimeout = 100; ///< ADC read timeout in milliseconds.
};

} // namespace sensor

#endif // SENSOR_HH
