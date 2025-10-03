#ifndef SOIL_HUM_SENSOR_HH
#define SOIL_HUM_SENSOR_HH

#include "../../sensor.hh"

namespace sensor {

/**
 * @class SoilHumSensor
 * @brief A class representing a soil humidity sensor.
 *
 * This class inherits from the Sensor base class and provides functionality
 * to read and process soil humidity data. It also includes methods for
 * calibration and retrieving humidity values.
 */
class SoilHumSensor : public Sensor {
  public:
    /**
     * @brief Constructor for SoilHumSensor.
     * @param channelConfig Optional ADC channel configuration.
     */
    SoilHumSensor(ADC_ChannelConfTypeDef *channelConfig = nullptr);
    virtual ~SoilHumSensor() override;

    /**
     * @brief Reads data from the sensor.
     */
    HAL_StatusTypeDef readData() override;

    /**
     * @brief Processes the data read from the sensor.
     */
    void processData() override;

    /**
     * @brief Gets the humidity percentage.
     * @return The humidity percentage as a float.
     */
    float getHumidityPercent() const;

    /**
     * @brief Checks if the humidity data is valid.
     * @return True if the data is valid, false otherwise.
     */
    bool isHumidityValid() const;

    /**
     * @brief Gets the raw sensor value.
     * @return The raw sensor value as a uint16_t.
     */
    uint16_t getRawValue() const;

    /**
     * @brief Calibrates the sensor with dry and wet values.
     * @param dryValue The raw value representing dry soil.
     * @param wetValue The raw value representing wet soil.
     */
    void calibrate(uint16_t dryValue, uint16_t wetValue);

  private:
    float m_humidityPercent = 0.0f;   ///< The calculated humidity percentage.
    uint16_t m_rawValue = 0;          ///< The raw value read from the sensor.
    uint16_t m_dryCalibration = 1023; ///< Calibration value for dry soil.
    uint16_t m_wetCalibration = 0;    ///< Calibration value for wet soil.
    bool m_dataValid = false;         ///< Flag indicating if the data is valid.
};

} // namespace sensor

#endif // SOIL_HUM_SENSOR_HH
