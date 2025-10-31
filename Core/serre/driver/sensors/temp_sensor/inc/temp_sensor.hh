#ifndef TEMP_SENSOR_HH
#define TEMP_SENSOR_HH

#include "../../inc/sensor.hh"

namespace sensor {

/**
 * @class TempSensor
 * @brief A class representing a temperature sensor, derived from the Sensor
 * base class.
 *
 * This class provides methods to read, process, and retrieve temperature data.
 * It also allows setting thresholds for temperature monitoring.
 */
class TempSensor final : public Sensor {
  public:
    TempSensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);
    ~TempSensor() override;

    /**
     * @brief Processes the raw data to calculate temperature.
     */
    void processData() override;

    /**
     * @brief Gets the temperature in Celsius.
     * @return Temperature in Celsius.
     */
    float getTemperatureCelsius() const;

    /**
     * @brief Gets the temperature in Fahrenheit.
     * @return Temperature in Fahrenheit.
     */
    float getTemperatureFahrenheit() const;

    /**
     * @brief Gets the temperature in Kelvin.
     * @return Temperature in Kelvin.
     */
    float getTemperatureKelvin() const;

    /**
     * @brief Checks if the temperature data is valid.
     * @return True if the data is valid, false otherwise.
     */
    bool isTemperatureValid() const;

    /**
     * @brief Sets the temperature thresholds.
     * @param minTemp Minimum temperature threshold.
     * @param maxTemp Maximum temperature threshold.
     */
    void setThreshold(float minTemp, float maxTemp);

  private:
    float m_temperature = 0.0f;    ///< Current temperature value.
    float m_minThreshold = -40.0f; ///< Minimum temperature threshold.
    float m_maxThreshold = 85.0f;  ///< Maximum temperature threshold.

    static constexpr float KELVIN_OFFSET = 273.15f;
    static constexpr float ADC_MAX_VALUE = 4095.0f; // Assuming a 12-bit ADC
    static constexpr float SENSOR_SLOPE = 100.0f; // Sensor output slope (°C/V)
};

} // namespace sensor

#endif // TEMP_SENSOR_HH
