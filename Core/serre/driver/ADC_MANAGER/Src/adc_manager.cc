/**
 * @brief ADC Manager class for handling ADC operations.
 * This class provides an interface to initialize and manage ADC
 * conversions for multiple channels using DMA.
 * @file adc_manager.cc
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 */

#include "../inc/adc_manager.hh"

namespace adc_manager {

ADCManager::ADCManager() {
}

void ADCManager::initialize(ADC_HandleTypeDef *adc_handle,
                            DMA_HandleTypeDef *dma_handle) {
    static ADCManager instance;
    instance.m_handle_.adc_handle = adc_handle;
    instance.m_handle_.dma_handle = dma_handle;
    instance.m_initialized_ = true;
}

ADCManager &ADCManager::getInstance() {
    static ADCManager instance;
    if (!instance.m_initialized_) {
        Error_Handler();
    }
    return instance;
}

HAL_StatusTypeDef ADCManager::start() {
    if (HAL_ADC_Start_DMA(m_handle_.adc_handle,
                          reinterpret_cast<uint32_t *>(this->m_buffer_),
                          BUFFER_LENGTH) != HAL_OK) {

        return HAL_ERROR;
    }
    return HAL_OK;
}

HAL_StatusTypeDef ADCManager::stop() {
    if (HAL_ADC_Stop_DMA(m_handle_.adc_handle) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint16_t ADCManager::getChannelValue(uint8_t channel) const {
    if (channel >= BUFFER_LENGTH) {
        return 0; // or handle error appropriately
    }
    return static_cast<uint16_t>(m_buffer_[channel] & 0x0FFF);
}

bool ADCManager::getConversionCompleteFlag() const volatile {
    return this->m_conversionCompleteFlag_;
}

void ADCManager::setConversionCompleteFlag(bool flag) volatile {
    this->m_conversionCompleteFlag_ = flag;
}

} // namespace adc_manager

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc == nullptr) {
        return;
    }

    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    adcManager.stop();
    adcManager.setConversionCompleteFlag(true);
}
