#include "../inc/sensor.hh"

namespace sensor {

Sensor::Sensor(adc_manager::ADCManager *adcManager, uint8_t numChannels) {
    if (adcManager == nullptr) {
        Error_Handler();
    }
    this->m_adcManager = adcManager;
    this->m_numChannels = numChannels;
}

HAL_StatusTypeDef Sensor::readData() {
    // Read raw ADC value from the sensor
    this->m_rawADC[this->m_sampleIndex] =
        this->m_adcManager->getChannelValue(this->m_numChannels);
    this->m_sampleIndex = (this->m_sampleIndex + 1) % 10;
    if (this->m_numSamples < 10) {
        this->m_numSamples++;
    }
    return HAL_OK;
}

void Sensor::processData() {
    // Average available samples
    uint8_t numSamples = this->m_numSamples > 0 ? this->m_numSamples : 1;
    this->m_processedValue = 0.0f;
    for (uint8_t i = 0; i < numSamples; i++) {
        this->m_processedValue += this->m_rawADC[i];
    }
    this->m_processedValue /= static_cast<float>(numSamples);
}

Sensor::~Sensor() {
}

} // namespace sensor
