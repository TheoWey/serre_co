#ifndef SOIL_HUM_SENSOR_HH
#define SOIL_HUM_SENSOR_HH

/**
 * @file soil_hum.hh
 * @brief Soil humidity sensor implementation.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the SoilHumSensor class which extends the Sensor
 * base class to provide soil humidity measurement and processing.
 */

#include "../../inc/sensor.hh"

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
 * @class SoilHumSensor
 * @brief A class representing a soil humidity sensor.
 *
 * This class inherits from the Sensor base class and provides functionality
 * to read and process soil humidity data. It includes methods for calibration
 * (dry/wet value mapping) and retrieving humidity percentages.
 *
 * @details The sensor uses a two-point calibration model where dry soil (0%)
 * and wet soil (100%) are mapped to raw ADC values. Raw measurements are
 * then converted to humidity percentage using linear interpolation.
 */
class SoilHumSensor final : public Sensor {
  public:
    /**
     * @brief Constructor for SoilHumSensor.
     *
     * @param adcManager Pointer to the ADCManager for ADC operations.
     * @param numChannels Number of ADC channels used by this sensor.
     *
     * @note The adcManager pointer must remain valid for the lifetime of this
     *       SoilHumSensor instance.
     */
    SoilHumSensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);
    /**
     * @brief Destructor for SoilHumSensor.
     *
     * Ensures proper cleanup of derived class resources.
     */
    ~SoilHumSensor() override;

    /**
     * @brief Process raw ADC data and calculate humidity percentage.
     *
     * Converts raw ADC values from the circular buffer to humidity percentage
     * using the calibration values (dry and wet points) with linear
     * interpolation. Updates m_humidityPercent and m_dataValid flag.
     *
     * @return void
     */
    void processData() override;

    /**
     * @brief Get the current humidity percentage.
     *
     * @return The humidity percentage in the range [0.0, 100.0].
     *
     * @note Check isHumidityValid() before using this value to ensure
     *       the data has been properly processed.
     */
    float getHumidityPercent() const;

    /**
     * @brief Check if the humidity data is valid.
     *
     * @return True if the data has been successfully processed and is valid,
     *         false if the data is stale or invalid.
     */
    bool isHumidityValid() const;

    /**
     * @brief Calibrate the sensor with dry and wet reference values.
     *
     * Establishes a two-point calibration model where dryValue corresponds to
     * 0% humidity and wetValue corresponds to 100% humidity. Subsequent raw
     * measurements are converted to percentage using linear interpolation
     * between these two calibration points.
     *
     * @param dryValue The raw ADC value representing dry soil (0% humidity).
     * @param wetValue The raw ADC value representing wet soil (100% humidity).
     *
     * @note Calibration should be performed before processing actual sensor
     * data. Ensure dryValue > wetValue for proper operation (typical for
     *       capacitive humidity sensors).
     */
    void calibrate(uint16_t dryValue, uint16_t wetValue);

    /**
     * @brief Retrieve the current calibration values.
     *
     * @param dryValue Reference to store the dry soil calibration value.
     * @param wetValue Reference to store the wet soil calibration value.
     *
     * @return void
     */
    inline void getCalibration(uint16_t &dryValue, uint16_t &wetValue) const {
        dryValue = this->m_dryCalibration;
        wetValue = this->m_wetCalibration;
    }

  private:
    float m_humidityPercent =
        0.0f; ///< Calculated humidity percentage [0.0, 100.0]
    uint16_t m_dryCalibration = 4095; ///< Raw ADC value for dry soil (0%)
    uint16_t m_wetCalibration = 0;    ///< Raw ADC value for wet soil (100%)
};

} // namespace sensor
} // namespace driver

#endif // SOIL_HUM_SENSOR_HH
