#include "sensor.hh"

namespace sensor {

HAL_StatusTypeDef Sensor::sensor_readHelper(uint16_t *outValue) {
    if (outValue == nullptr) {
        return HAL_ERROR;
    }

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = this->m_config.adcChannel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = this->m_config.adcSamplingTime;

    if (HAL_ADC_ConfigChannel(this->m_config.adcHandle, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADC_Start(this->m_config.adcHandle) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_ADC_PollForConversion(this->m_config.adcHandle,
                                  this->m_config.adcTimeout) != HAL_OK) {
        HAL_ADC_Stop(this->m_config.adcHandle);
        return HAL_TIMEOUT;
    }
    *outValue = HAL_ADC_GetValue(this->m_config.adcHandle);
    HAL_ADC_Stop(this->m_config.adcHandle);

    return HAL_OK;
}

HAL_StatusTypeDef Sensor::readData() {
    // Read raw ADC value from the sensor
    if (this->sensor_readHelper(&(this->m_rawADC[this->m_sampleIndex])) !=
        HAL_OK) {
        return HAL_ERROR;
    } else {
        this->m_sampleIndex = (this->m_sampleIndex + 1) % 10;
        if (this->m_numSamples < 10) {
            this->m_numSamples++;
        }
        return HAL_OK;
    }
}

void Sensor::processData() {
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
