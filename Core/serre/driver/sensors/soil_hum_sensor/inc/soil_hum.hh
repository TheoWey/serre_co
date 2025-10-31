#ifndef SOIL_HUM_SENSOR_HH
#define SOIL_HUM_SENSOR_HH

#include "../../inc/sensor.hh"

namespace sensor {

/**
 * @class SoilHumSensor
 * @brief A class representing a soil humidity sensor.
 *
 * This class inherits from the Sensor base class and provides functionality
 * to read and process soil humidity data. It also includes methods for
 * calibration and retrieving humidity values.
 */
class SoilHumSensor final : public Sensor {
  public:
    SoilHumSensor(adc_manager::ADCManager *adcManager, uint8_t numChannels);
    ~SoilHumSensor() override;

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
     * @brief Calibrates the sensor with dry and wet values.
     * @param dryValue The raw value representing dry soil.
     * @param wetValue The raw value representing wet soil.
     */
    void calibrate(uint16_t dryValue, uint16_t wetValue);

  private:
    float m_humidityPercent = 0.0f;   ///< The calculated humidity percentage.
    uint16_t m_dryCalibration = 4095; ///< Calibration value for dry soil.
    uint16_t m_wetCalibration = 0;    ///< Calibration value for wet soil.
};

} // namespace sensor

#endif // SOIL_HUM_SENSOR_HH
