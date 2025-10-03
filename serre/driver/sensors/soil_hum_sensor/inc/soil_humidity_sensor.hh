#ifndef SOIL_HUM_SENSOR_HH
#define SOIL_HUM_SENSOR_HH

#include "sensor.hh"

namespace sensor {

class SoilHumSensor : public Sensor {
  public:
    SoilHumSensor();
    virtual ~SoilHumSensor() override;

    void init() override;
    void readData() override;
    void processData() override;

    // Methods specific to soil humidity
    float getHumidityPercent() const;
    bool isHumidityValid() const;
    uint16_t getRawValue() const;
    void calibrate(uint16_t dryValue, uint16_t wetValue);

  private:
    void configure() override;

    float m_humidityPercent = 0.0f;
    uint16_t m_rawValue = 0;
    uint16_t m_dryCalibration = 0;
    uint16_t m_wetCalibration = 0;
    bool m_dataValid = false;
};

} // namespace sensor

#endif // SOIL_HUM_SENSOR_HH