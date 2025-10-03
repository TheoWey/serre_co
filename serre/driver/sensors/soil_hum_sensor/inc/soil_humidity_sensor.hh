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

    float m_humidityPercent;
    uint16_t m_rawValue;
    uint16_t m_dryCalibration;
    uint16_t m_wetCalibration;
    bool m_dataValid;
};

} // namespace sensor

#endif // SOIL_HUM_SENSOR_HH