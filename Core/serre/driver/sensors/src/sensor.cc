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
    this->m_rawADC.add(
        this->m_adcManager->getChannelValue(this->m_numChannels));
    if (this->m_numSamples < 10) {
        this->m_numSamples++;
    }
    return HAL_OK;
}

void Sensor::processData() {
    uint8_t numSamples = this->m_numSamples > 0 ? this->m_numSamples : 1;

    uint16_t sum = 0;
    for (uint8_t i = 0; i < numSamples; i++) {
        uint16_t value;
        if (this->m_rawADC.get(value)) {
            sum += value;
        }
    }

    this->m_processedValue =
        static_cast<float>(sum) / static_cast<float>(numSamples);
}

Sensor::~Sensor() {
}

} // namespace sensor
