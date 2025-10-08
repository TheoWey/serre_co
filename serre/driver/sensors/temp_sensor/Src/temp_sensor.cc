#include "../inc/temp_sensor.hh"

namespace sensor {

TempSensor::TempSensor(ADC_HandleTypeDef *adcHandle) {
    // Initialize the ADC channel configuration
    if (adcHandle != nullptr) {
        // Assuming m_adcHandle is properly initialized elsewhere
        this->m_adcHandle = adcHandle;
    }
}
HAL_StatusTypeDef TempSensor::readData() {
    // Read raw ADC value from the sensor
    this->m_sampleIndex = (this->m_sampleIndex + 1) % 10;
    return this->sensor_readHelper(
        reinterpret_cast<uint32_t *>(&(this->m_rawADC[this->m_sampleIndex])),
        this->m_adcTimeout);
}

void TempSensor::processData() {
    sensor::Sensor::processData();
    this->m_temperature =
        (static_cast<float>(this->m_processedValue) / this->ADC_MAX_VALUE) *
        this->SENSOR_MAX_TEMP;
    // Validate the temperature data
    this->m_dataValid = (this->m_temperature >= this->m_minThreshold) &&
                        (this->m_temperature <= this->m_maxThreshold);
}

float TempSensor::getTemperatureCelsius() const {
    return this->m_temperature;
}

float TempSensor::getTemperatureFahrenheit() const {
    return this->m_temperature * (9.0f / 5.0f) + 32.0f;
}

float TempSensor::getTemperatureKelvin() const {
    return this->m_temperature + this->KELVIN_OFFSET;
}

bool TempSensor::isTemperatureValid() const {
    return this->m_dataValid;
}

void TempSensor::setThreshold(float minTemp, float maxTemp) {
    this->m_minThreshold = minTemp; 
    this->m_maxThreshold = maxTemp; 
}

TempSensor::~TempSensor() {
}
} // namespace sensor
