/**
 * @brief ADC Manager class for handling ADC operations.
 * This class provides an interface to initialize and manage ADC
 * conversions for multiple channels using DMA.
 * @file adc_manager.hh
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 */
#ifndef ADC_MANAGER_HH
#define ADC_MANAGER_HH

#include "../../../../Inc/adc.h"
#include "../../../../Inc/dma.h"

namespace adc_manager {
/**
 * @brief HAL-like ADC Manager handle placeholder.
 *
 * The project declares a placeholder for the ADC Manager hardware handle.
 * This structure holds pointers to the ADC and DMA handles used for
 * multi-channel conversions.
 */
const uint8_t BUFFER_LENGTH = 2; // Number of regular channels scanned

struct ADCManagerHandle {
    ADC_HandleTypeDef *adc_handle;
    DMA_HandleTypeDef *dma_handle;
};

class ADCManager {
  public:
    static void initialize(ADC_HandleTypeDef *adc_handle,
                           DMA_HandleTypeDef *dma_handle);

    /**
     * @brief Get the singleton instance of ADCManager.
     * @param adc_handle Pointer to the ADC handle (optional for first call).
     * @param dma_handle Pointer to the DMA handle (optional for first call).
     * @return Reference to the singleton ADCManager instance.
     */
    static ADCManager &getInstance();
    /**
     * @brief Start ADC conversions using DMA.
     * @return HAL_StatusTypeDef indicating the status of the operation.
     */
    HAL_StatusTypeDef start();

    /**
     * @brief Stop ADC conversions.
     * @return HAL_StatusTypeDef indicating the status of the operation.
     */
    HAL_StatusTypeDef stop();

    /**
     * @brief Get the ADC conversion result for a specific channel.
     * @return The ADC value for the specified channel index.
     */
    uint16_t getChannelValue(uint8_t index) const;

    bool getConversionCompleteFlag() const volatile;

    void setConversionCompleteFlag(bool flag) volatile;

  private:
    /**
     * @brief Constructor for ADCManager.
     * @param adc_handle Pointer to the ADC handle.
     * @param dma_handle Pointer to the DMA handle.
     */
    ADCManager();
    ADCManager(const ADCManager &) = delete;
    ADCManager &operator=(const ADCManager &) = delete;

    bool m_initialized_ = false;
    ADCManagerHandle m_handle_;
    volatile bool m_conversionCompleteFlag_ = false;
    uint16_t m_buffer_[BUFFER_LENGTH]; // DMA writes 12-bit right-aligned in
                                       // 16-bit halfwords
};

} // namespace adc_manager

#endif // ADC_MANAGER_HH
