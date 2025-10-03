#include "../inc/temp_sensor.hh"

namespace sensor {
TempSensor::TempSensor(ADC_HandleTypeDef *m_channel) {
    // Initialize the ADC channel configuration
    if (m_channel != nullptr) {
        // Assuming m_channel is properly initialized elsewhere
        this->m_channel = m_channel;
    }
}
HAL_StatusTypeDef TempSensor::readData() {
    // Read raw ADC value from the sensor
    this->m_sampleIndex = (this->m_sampleIndex + 1) % 10;
    return this->sensor_readHelper(
        reinterpret_cast<uint32_t *>(&(this->m_rawADC[this->m_sampleIndex])),
        100);
}

void TempSensor::processData() {
    // Convert raw ADC value to temperature in Kelvin
    this->m_temperature = 0.0f;
    for (uint8_t i = 0; i < 10; i++) {
        this->m_temperature +=
            (static_cast<float>(this->m_rawADC[i]) / 4095.0f) * 100.0f +
            273.15f;
    }
    this->m_temperature /= 10.0f;
    // Validate the temperature data
    this->m_dataValid = (this->m_temperature >= this->m_minThreshold) &&
                        (this->m_temperature <= this->m_maxThreshold);
}

float TempSensor::getTemperatureCelsius() const {
    return this->m_temperature - 273.15f;
}

float TempSensor::getTemperatureFahrenheit() const {
    return (this->m_temperature - 273.15f) * 9.0f / 5.0f + 32.0f;
}

bool TempSensor::isTemperatureValid() const {
    return this->m_dataValid;
}

void TempSensor::setThreshold(float minTemp, float maxTemp) {
    this->m_minThreshold = minTemp + 273.15f; // Convert to Kelvin
    this->m_maxThreshold = maxTemp + 273.15f; // Convert to Kelvin
}

TempSensor::~TempSensor() {
}
} // namespace sensor
