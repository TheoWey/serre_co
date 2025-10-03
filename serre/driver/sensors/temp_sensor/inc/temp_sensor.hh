#ifndef TEMP_SENSOR_HH
#define TEMP_SENSOR_HH

#include "../../sensor.hh"

namespace sensor {

/**
 * @class TempSensor
 * @brief A class representing a temperature sensor, derived from the Sensor
 * base class.
 *
 * This class provides methods to read, process, and retrieve temperature data.
 * It also allows setting thresholds for temperature monitoring.
 */
class TempSensor : public Sensor {
  public:
    /**
     * @brief Constructor for TempSensor.
     * @param channelConfig Pointer to ADC channel configuration (default:
     * nullptr).
     */
    TempSensor(ADC_HandleTypeDef *m_channel = nullptr);
    virtual ~TempSensor() override;

    /**
     * @brief Reads data from the temperature sensor.
     */
    HAL_StatusTypeDef readData() override;

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
    bool m_dataValid = false;      ///< Flag indicating if the data is valid.
    uint16_t m_rawADC[10] = {0};   ///< Raw ADC value from the sensor.
    uint8_t m_sampleIndex = 0;     ///< Index for sampling multiple readings.
};

} // namespace sensor

#endif // TEMP_SENSOR_HH
