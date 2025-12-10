#ifndef TEMP_SENSOR_HH
#define TEMP_SENSOR_HH

/**
 * @file temp_sensor.hh
 * @brief Temperature sensor implementation.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the TempSensor class which extends the Sensor base
 * class to provide temperature measurement and processing with multiple
 * temperature scale outputs.
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
 * @class TempSensor
 * @brief A class representing a temperature sensor.
 *
 * Derived from the Sensor base class, this class provides methods to read,
 * process, and retrieve temperature data in multiple scales (Celsius,
 * Fahrenheit, Kelvin). It supports temperature threshold monitoring and
 * calibration offset adjustment.
 *
 * @details The sensor converts raw ADC values to temperature using a linear
 * calibration model with slope and offset parameters. Temperature data can be
 * retrieved in three different units, and validity can be checked before use.
 */
class TempSensor final : public Sensor {
  public:
    /**
     * @brief Constructor for TempSensor.
     *
     * @param adcManager Pointer to the ADCManager for ADC operations.
     * @param numChannels Number of ADC channels used by this sensor.
     *
     * @note The adcManager pointer must remain valid for the lifetime of this
     *       TempSensor instance.
     */
    TempSensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);
    /**
     * @brief Destructor for TempSensor.
     *
     * Ensures proper cleanup of derived class resources.
     */
    ~TempSensor() override;

    /**
     * @brief Process raw ADC data and calculate temperature in Celsius.
     *
     * Converts raw ADC values from the circular buffer to temperature using
     * the linear calibration model (SENSOR_SLOPE, offset, ADC_MAX_VALUE).
     * Updates m_temperature and m_dataValid flag.
     *
     * @return void
     */
    void processData() override;

    /**
     * @brief Get the current temperature in Celsius.
     *
     * @return Temperature in Celsius (°C).
     *
     * @note Check isTemperatureValid() before using this value to ensure
     *       the data has been properly processed.
     */
    float getTemperatureCelsius() const;

    /**
     * @brief Get the current temperature in Fahrenheit.
     *
     * Converts the internal Celsius temperature to Fahrenheit using the
     * formula: °F = (°C × 9/5) + 32
     *
     * @return Temperature in Fahrenheit (°F).
     */
    float getTemperatureFahrenheit() const;

    /**
     * @brief Get the current temperature in Kelvin.
     *
     * Converts the internal Celsius temperature to Kelvin using the formula:
     * K = °C + 273.15
     *
     * @return Temperature in Kelvin (K).
     */
    float getTemperatureKelvin() const;

    /**
     * @brief Check if the temperature data is valid.
     *
     * @return True if the data has been successfully processed and is valid,
     *         false if the data is stale or invalid.
     */
    bool isTemperatureValid() const;

    /**
     * @brief Set temperature threshold limits.
     *
     * Configures the minimum and maximum temperature thresholds. These can be
     * used for alarm generation or control logic when temperature exceeds the
     * specified bounds.
     *
     * @param minTemp Minimum temperature threshold in Celsius.
     * @param maxTemp Maximum temperature threshold in Celsius.
     *
     * @note Ensure minTemp < maxTemp. Default thresholds are [-40°C, 85°C].
     */
    void setThreshold(float minTemp, float maxTemp);

    /**
     * @brief Set calibration offset using a known reference temperature.
     *
     * Calculates and stores the calibration offset by comparing the measured
     * temperature with a known real temperature value.
     *
     * @param real_temp Known real temperature in Celsius for calibration.
     *
     * @return void
     *
     * @note Should be called during calibration phase with a known temperature
     *       source (e.g., ice bath at 0°C or boiling water at 100°C).
     */
    inline void setOffset(uint16_t real_temp) {
        this->m_offset = this->m_temperature - real_temp;
    }

  private:
    float m_temperature = 0.0f;    ///< Current temperature value in Celsius
    float m_minThreshold = -40.0f; ///< Minimum temperature threshold in °C
    float m_maxThreshold = 85.0f;  ///< Maximum temperature threshold in °C
    int16_t m_offset = 0;          ///< Calibration offset in sensor units

    static constexpr float KELVIN_OFFSET =
        273.15f; ///< Offset to convert Celsius to Kelvin
    static constexpr float ADC_MAX_VALUE =
        4095.0f; ///< Maximum value for 12-bit ADC
    static constexpr float SENSOR_SLOPE =
        100.0f; ///< Sensor output slope in °C/V
};

} // namespace sensor
} // namespace driver

#endif // TEMP_SENSOR_HH
