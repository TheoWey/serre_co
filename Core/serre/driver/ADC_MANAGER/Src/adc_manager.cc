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

__WEAK void init_adc(void) {
    // Weak implementation for user override
}

void ADCManager::initialize(ADC_HandleTypeDef *adc_handle,
                            DMA_HandleTypeDef *dma_handle,
                            TIM_HandleTypeDef *tim_handle) {
    ADCManager &instance = getInstance();
    instance.adc_handler.adc_handle = adc_handle;
    instance.adc_handler.dma_handle = dma_handle;
    instance.adc_handler.tim_handle = tim_handle;
}

ADCManager &ADCManager::getInstance() {
    static ADCManager instance;
    return instance;
}

HAL_StatusTypeDef ADCManager::start() {
    HAL_TIM_Base_Start(adc_handler.tim_handle);
    if (HAL_ADC_Start_DMA(adc_handler.adc_handle,
                          reinterpret_cast<uint32_t *>(this->m_buffer_),
                          BUFFER_LENGTH) != HAL_OK) {

        return HAL_ERROR;
    }
    return HAL_OK;
}

HAL_StatusTypeDef ADCManager::stop() {
    if (HAL_ADC_Stop_DMA(adc_handler.adc_handle) != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_TIM_Base_Stop(adc_handler.tim_handle);
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

    adc_manager::ADCManager::getInstance().setConversionCompleteFlag(true);
}
