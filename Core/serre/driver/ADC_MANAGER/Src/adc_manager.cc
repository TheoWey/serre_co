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
ADCManager *ADCManager::m_instance = nullptr;

ADCManager::ADCManager() {
}

void ADCManager::initialize(ADC_HandleTypeDef *adc_handle,
                            DMA_HandleTypeDef *dma_handle,
                            TIM_HandleTypeDef *tim_handle) {
    static ADCManager instance;
    instance.m_handle_.adc_handle = adc_handle;
    instance.m_handle_.dma_handle = dma_handle;
    instance.m_handle_.tim_handle = tim_handle;
    instance.m_initialized_ = true;
    m_instance = &instance;
}

ADCManager &ADCManager::getInstance() {
    // Ensure initialize() has been called before use
    if (m_instance == nullptr || !m_instance->m_initialized_) {
        Error_Handler();
    }
    return *m_instance;
}

HAL_StatusTypeDef ADCManager::start() {
    HAL_TIM_Base_Start(m_handle_.tim_handle);
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
    HAL_TIM_Base_Stop(m_handle_.tim_handle);
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
