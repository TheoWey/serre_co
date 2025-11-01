#include "../inc/temp_sensor.hh"

namespace sensor {

TempSensor::TempSensor(adc_manager::ADCManager *adcManager, uint8_t numChannels)
    : Sensor(adcManager, numChannels) {
    // Initialize the ADC channel configuration
    if (adcManager == nullptr) {
        Error_Handler();
    }
}

void TempSensor::processData() {
    sensor::Sensor::processData();
    this->m_processedValue = (3.3f * this->m_processedValue) / ADC_MAX_VALUE;
    this->m_temperature =
        (static_cast<float>(this->m_processedValue) - 0.5) * this->SENSOR_SLOPE;
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
