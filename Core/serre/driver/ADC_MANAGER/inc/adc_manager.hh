/**
 * @file adc_manager.hh
 * @brief ADC manager for multi-channel conversions via DMA.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 *
 * Declares the ADCManager singleton used to configure and run ADC conversions
 * triggered by a timer and transferred through DMA. Provides access to the
 * latest sampled values and a conversion completion flag.
 */
#ifndef ADC_MANAGER_HH
#define ADC_MANAGER_HH

#include "../../../../Inc/adc.h"
#include "../../../../Inc/dma.h"
#include "../../../../Inc/tim.h"

/**
 * @namespace driver
 * @brief Contains classes and methods for various drivers.
 */
namespace driver {
/**
 * @namespace adc_manager
 * @brief Contains classes and methods for handling ADC operations.
 */
namespace adc_manager {

/**
 * @brief HAL-like ADC Manager handle placeholder.
 *
 * The project declares a placeholder for the ADC Manager hardware handle.
 * This structure holds pointers to the ADC and DMA handles used for
 * multi-channel conversions.
 */
/**
 * @brief Number of regular ADC channels scanned in the DMA buffer.
 */
const uint8_t BUFFER_LENGTH = 2;

struct ADCManagerHandle {
    ADC_HandleTypeDef *adc_handle; ///< Pointer to the ADC peripheral handle
    DMA_HandleTypeDef *dma_handle; ///< Pointer to the DMA handle used for ADC
    TIM_HandleTypeDef *tim_handle; ///< Pointer to the timer used as trigger
};

/**
 * @brief Initializes the ADC subsystem. Implemented as a weak function in
 * adc_manager.cc.
 */
void init_adc(void);

class ADCManager {
  public:
    /**
     * @brief Initialize the ADC manager with peripheral handles.
     *
     * Copies the provided handles and prepares the manager for DMA-driven
     * conversions.
     *
     * @param adc_handle Pointer to the ADC handle.
     * @param dma_handle Pointer to the DMA handle.
     * @param tim_handle Pointer to the TIM handle used as trigger source.
     */
    static void initialize(ADC_HandleTypeDef *adc_handle,
                           DMA_HandleTypeDef *dma_handle,
                           TIM_HandleTypeDef *tim_handle);

    /**
     * @brief Get the singleton instance of ADCManager.
     *
     * @return Reference to the singleton ADCManager instance.
     * @note initialize() must be called before first use.
     */
    static ADCManager &getInstance();

    /**
     * @brief Start ADC conversions using DMA (timer-triggered).
     * @return HAL status of the start operation.
     */
    HAL_StatusTypeDef start();

    /**
     * @brief Stop ADC conversions and DMA transfers.
     * @return HAL status of the stop operation.
     */
    HAL_StatusTypeDef stop();

    /**
     * @brief Get the latest ADC value for a channel.
     * @param index Channel index (0..BUFFER_LENGTH-1).
     * @return Latest sampled ADC value for the given channel.
     */
    uint16_t getChannelValue(uint8_t index) const;

    /**
     * @brief Check if a DMA conversion has completed.
     * @return True if conversion is complete, false otherwise.
     */
    bool getConversionCompleteFlag() const volatile;

    /**
     * @brief Set or clear the conversion complete flag.
     * @param flag New flag state.
     */
    void setConversionCompleteFlag(bool flag) volatile;

  private:
    ADCManager() = default;
    ADCManager(const ADCManager &) = delete;
    ADCManager &operator=(const ADCManager &) = delete;

    ADCManagerHandle adc_handler; ///< Stored peripheral handles
    volatile bool m_conversionCompleteFlag_ = false; ///< DMA completion flag
    uint16_t m_buffer_[BUFFER_LENGTH]; ///< DMA buffer for ADC values
};

} // namespace adc_manager
} // namespace driver

#endif // ADC_MANAGER_HH
