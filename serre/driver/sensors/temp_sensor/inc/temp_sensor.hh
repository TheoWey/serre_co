#ifndef TEMP_SENSOR_HH
#define TEMP_SENSOR_HH

#include "sensor.hh"

namespace sensor {

class TempSensor : public Sensor {
  public:
    TempSensor();
    virtual ~TempSensor() override;

    void init() override;
    void readData() override;
    void processData() override;

    // Temperature-specific methods
    float getTemperatureCelsius() const;
    float getTemperatureFahrenheit() const;
    bool isTemperatureValid() const;
    void setThreshold(float minTemp, float maxTemp);

  private:
    void configure() override;

    float m_temperature;
    float m_minThreshold;
    float m_maxThreshold;
    bool m_dataValid;
    uint16_t m_rawADC;
};

} // namespace sensor

#endif // TEMP_SENSOR_HH