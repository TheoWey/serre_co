#include "../inc/soil_hum.hh"

namespace sensor {
SoilHumSensor::SoilHumSensor(ADC_HandleTypeDef *adcHandle) {
    // Initialize the ADC channel configuration
    if (adcHandle != nullptr) {
        // Assuming m_adcHandle is properly initialized elsewhere
        this->m_adcHandle = adcHandle;
    }
}

HAL_StatusTypeDef SoilHumSensor::readData() {
    // Read raw ADC value from the sensor
    this->m_sampleIndex = (this->m_sampleIndex + 1) % 10;
    return this->sensor_readHelper(
        reinterpret_cast<uint32_t *>(&(this->m_rawADC[this->m_sampleIndex])),
        this->m_adcTimeout);
}

void SoilHumSensor::processData() {
    sensor::Sensor::processData();
    // Calculate humidity percentage based on calibration values
    if (this->m_processedValue <= this->m_wetCalibration) {
        this->m_humidityPercent = 100.0f;
        this->m_dataValid = true;
    } else if (this->m_processedValue >= this->m_dryCalibration) {
        this->m_humidityPercent = 0.0f;
        this->m_dataValid = true;
    } else {
        this->m_humidityPercent =
            100.0f - ((static_cast<float>(this->m_processedValue) -
                   static_cast<float>(this->m_wetCalibration)) /
                  (static_cast<float>(this->m_dryCalibration) -
                   static_cast<float>(this->m_wetCalibration))) *
                 100.0f;
        this->m_dataValid = true;
    }
}
float SoilHumSensor::getHumidityPercent() const {
    return this->m_humidityPercent;
}

bool SoilHumSensor::isHumidityValid() const {
    return this->m_dataValid;
}

void SoilHumSensor::calibrate(uint16_t dryValue, uint16_t wetValue) {
    this->m_dryCalibration = dryValue;
    this->m_wetCalibration = wetValue;
}

SoilHumSensor::~SoilHumSensor() {
}

} // namespace sensor
