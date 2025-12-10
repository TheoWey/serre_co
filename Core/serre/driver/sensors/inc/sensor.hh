#ifndef SENSOR_HH
#define SENSOR_HH

/**
 * @file sensor.hh
 * @brief Abstract base class for sensor management.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 */

// Includes
#include "../../../UTILS/circular_buffer/circular_buffer.hh"
#include "../../ADC_MANAGER/inc/adc_manager.hh"

/**
 * @namespace driver
 * @brief Contains classes and methods for various drivers.
 */
namespace driver {
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
 * data from sensors. It integrates with the ADC manager for analog data
 * acquisition and maintains a circular buffer for raw sample storage.
 *
 * @details Derived classes must implement the virtual methods to provide
 * specific sensor functionality. The class handles raw ADC data through
 * a circular buffer and provides a processedValue field for calibrated
 * or converted sensor readings.
 *
 * @note Derived classes must implement the virtual methods to provide specific
 *       sensor functionality.
 */
class Sensor {
  public:
    /**
     * @brief Constructor for Sensor class.
     *
     * @param adcManager Pointer to the ADCManager instance for ADC operations.
     * @param numChannels Number of ADC channels used by this sensor.
     *
     * @note The adcManager pointer must remain valid for the lifetime of this
     *       Sensor instance.
     */
    Sensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);

    /**
     * @brief Virtual destructor for Sensor.
     *
     * Ensures proper cleanup of derived class resources.
     */
    virtual ~Sensor();

    /**
     * @brief Read data from the sensor via ADC channels.
     *
     * Initiates ADC conversion through the ADCManager and stores raw values
     * in the circular buffer.
     *
     * @return HAL_StatusTypeDef Status of the ADC read operation (HAL_OK,
     * HAL_ERROR, etc.).
     */
    HAL_StatusTypeDef readData();

    /**
     * @brief Process the raw ADC data and update the processed value.
     *
     * Converts raw ADC samples from the circular buffer to engineering units
     * or applies calibration/filtering as needed. Must be implemented by
     * derived classes for specific sensor types.
     *
     * @return void
     */
    virtual void processData();

    /**
     * @brief Calibrate the sensor offset.
     *
     * Adjusts the sensor readings to account for manufacturing tolerances,
     * temperature drift, or other systematic errors. Implementation is
     * provided by derived classes.
     *
     * @return void
     */
    virtual void calibrateOffset();

  protected:
    adc_manager::ADCManager
        *m_adcManager;         ///< Pointer to ADCManager for data acquisition
    uint8_t m_numChannels = 0; ///< Number of ADC channels used by this sensor

    bool m_dataValid = false; ///< Flag indicating if current data is valid
    const static uint8_t MAX_SAMPLES =
        10; ///< Maximum number of samples in circular buffer
    utils::circular_buffer::CircularBuffer<uint16_t, MAX_SAMPLES>
        m_rawADC;               ///< Circular buffer storing raw ADC values
    float m_processedValue = 0; ///< Processed sensor value in engineering units
};

} // namespace sensor
} // namespace driver
#endif // SENSOR_HH
