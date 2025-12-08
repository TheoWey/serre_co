#include "../inc/sensor.hh"

namespace driver {
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
    return HAL_OK;
}

void Sensor::processData() {
    // Simple average processing of the raw ADC values
    this->m_processedValue = this->m_rawADC.average();
}

void Sensor::calibrateOffset() {
    // Default implementation does nothing
}

Sensor::~Sensor() {
}

} // namespace sensor
} // namespace driver
