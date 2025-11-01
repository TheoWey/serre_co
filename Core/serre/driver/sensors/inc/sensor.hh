#ifndef SENSOR_HH
#define SENSOR_HH

// Includes
#include "../../ADC_MANAGER/inc/adc_manager.hh"
#include "../../UTILS/circular_buffer/circular_buffer.hh"

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
 * data from sensors. It includes a configuration structure for sensor
 * parameters, including timeout configuration.
 *
 * @note Derived classes must implement the virtual methods to provide specific
 *       sensor functionality.
 */
class Sensor {
  public:
    /**
     * @brief Constructor for Sensor class.
     * @param config Configuration structure for the sensor.
     */
    Sensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);

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
    adc_manager::ADCManager *m_adcManager;
    uint8_t m_numChannels = 0; ///< Number of ADC channels used by the sensor.

    uint8_t m_numSamples = 0; ///< Number of samples to average.
    bool m_dataValid = false; ///< Flag indicating if the data is valid.
    const static uint8_t MAX_SAMPLES = 10; ///< Maximum number of samples.
    utils::CircularBuffer<uint16_t, MAX_SAMPLES>
        m_rawADC;               ///< Raw ADC value from the sensor.
    float m_processedValue = 0; ///< Processed sensor value.
};

} // namespace sensor

#endif // SENSOR_HH
