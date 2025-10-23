#ifndef SENSOR_HH
#define SENSOR_HH

// Includes
#include "../../../Inc/adc.h"
/**
 * @namespace sensor
 * @brief Contains classes and methods for handling various sensors.
 */

namespace sensor {

/**
 * @brief Configuration structure for an analog sensor using ADC.
 *
 * This structure holds the configuration parameters required to interface
 * with an analog sensor via the STM32 ADC peripheral.
 *
 * @struct SensorConfig
 * @var ADC_HandleTypeDef *adcHandle
 *      Pointer to the ADC handle used for sensor readings.
 * @var uint32_t adcChannel
 *      ADC channel number assigned to the sensor.
 * @var uint32_t adcSamplingTime
 *      ADC sampling time configuration for the sensor.
 * @var uint32_t adcTimeout
 *      Timeout duration (in milliseconds) for ADC read operations.
 */
typedef struct {
    ADC_HandleTypeDef *adcHandle; ///< Pointer to the ADC handle
    uint32_t adcChannel;          ///< ADC channel number
    uint32_t adcSamplingTime;     ///< ADC sampling time
    uint32_t adcTimeout;          ///< ADC read timeout in milliseconds
} SensorConfig;

/**
 * @class Sensor
 * @brief Abstract base class for sensor management.
 *
 * This class provides an interface for initializing, reading, and processing
 * data from sensors. It includes a configuration structure for sensor
 * parameters, including timeout configuration.
 *
 * @note Derived classes must implement the virtual methods to provide specific
 *       sensor functionality.
 */
class Sensor {
  public:
    /**
     * @brief Virtual destructor for Sensor.
     */
    virtual ~Sensor();
    
    /**
     * @brief Reads data from the temperature sensor.
     */
    HAL_StatusTypeDef readData();

    /**
     * @brief Processes the raw ADC data and updates the processed value.
     */
    virtual void processData();

  protected:
    /**
     * @brief Helper function to read a value from the sensor's ADC.
     * @param[out] outValue Pointer to store the read ADC value.
     * @return HAL status of the read operation.
     */
    HAL_StatusTypeDef sensor_readHelper(uint16_t *outValue);


    /**
     * @brief Sensor configuration structure.
     */
    SensorConfig m_config = {};

    uint8_t m_numSamples = 0;    ///< Number of samples to average.
    bool m_dataValid = false;    ///< Flag indicating if the data is valid.
    uint16_t m_rawADC[10] = {0}; ///< Raw ADC value from the sensor.
    float m_processedValue = 0;  ///< Processed sensor value.
    uint8_t m_sampleIndex = 0;   ///< Index for sampling multiple readings.
};

} // namespace sensor

#endif // SENSOR_HH
