/**
 * @brief ADC Manager class for handling ADC operations.
 * This class provides an interface to initialize and manage ADC
 * conversions for multiple channels using DMA.
 * @file adc_manager.hh
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 *
 */
#ifndef ADC_MANAGER_HH
#define ADC_MANAGER_HH

#include "../../../../Inc/adc.h"
#include "../../../../Inc/dma.h"
#include "../../../../Inc/tim.h"

namespace driver {
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
    TIM_HandleTypeDef *tim_handle;
};

/**
 * @brief Initializes the ADC subsystem. Implemented as a weak function in
 * adc_manager.cc.
 */
void init_adc(void);

class ADCManager {
  public:
    /**
     * @brief Initialize the ADC manager with given handles.
     *
     * @param adc_handle Pointer to the ADC handle.
     * @param dma_handle Pointer to the DMA handle.
     * @param tim_handle Pointer to the TIM handle for triggering.
     */
    static void initialize(ADC_HandleTypeDef *adc_handle,
                           DMA_HandleTypeDef *dma_handle,
                           TIM_HandleTypeDef *tim_handle);

    /**
     * @brief Get the singleton instance of ADCManager.
     *
     * @return Reference to the singleton ADCManager instance.
     *
     * @note initialize() must be called before first use of getInstance().
     */
    static ADCManager &getInstance();

    /**
     * @brief Start ADC conversions using DMA.
     *
     * @return HAL_StatusTypeDef indicating the status of the operation.
     */
    HAL_StatusTypeDef start();

    /**
     * @brief Stop ADC conversions.
     *
     * @return HAL_StatusTypeDef indicating the status of the operation.
     */
    HAL_StatusTypeDef stop();

    /**
     * @brief Get the ADC conversion result for a specific channel.
     *
     * @param index Channel index (0..BUFFER_LENGTH-1).
     * @return The ADC value for the specified channel index.
     */
    uint16_t getChannelValue(uint8_t index) const;

    /**
     * @brief Get the conversion complete flag.
     *
     * @return True if conversion is complete, false otherwise.
     */
    bool getConversionCompleteFlag() const volatile;

    /**
     * @brief Set the conversion complete flag.
     *
     * @param flag Flag value to set.
     */
    void setConversionCompleteFlag(bool flag) volatile;

  private:
    ADCManager() = default;
    ADCManager(const ADCManager &) = delete;
    ADCManager &operator=(const ADCManager &) = delete;

    ADCManagerHandle adc_handler;
    volatile bool m_conversionCompleteFlag_ = false;
    uint16_t m_buffer_[BUFFER_LENGTH]; /**< DMA buffer for ADC values */
};

} // namespace adc_manager
} // namespace driver

#endif // ADC_MANAGER_HH
