#ifndef __I2C_MANAGER_HH__
#define __I2C_MANAGER_HH__
/**
 * @file i2c_manager.hh
 * @brief I2C Manager Header File
 * @author ThéoWey ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 * @details
 * This header file defines the interface for the I2C manager, including
 * initialization and basic operations. It supports I2C communication using
 * STM32 HAL types.
 * @note This file has to be updated for correct non-blocking mode
 * implementation. Currently, only blocking mode is supported even if DMA is
 * active and implemented.
 */

#include "../../../../Inc/i2c.h"

/**
 * @brief Check whether an I2C device is ready.
 *
 * Wrapper macro around HAL_I2C_IsDeviceReady that uses the fields of an
 * i2c_handler_t pointer.
 *
 * @param i2c_handler_ptr Pointer to i2c_handler_t.
 * @return HAL status as returned by HAL_I2C_IsDeviceReady.
 */
#define ping(i2c_handler_ptr)                                                  \
    HAL_I2C_IsDeviceReady((i2c_handler_ptr).hi2c, (i2c_handler_ptr).address,   \
                          (i2c_handler_ptr).timeout, (i2c_handler_ptr).retry)

namespace i2c {

#define BLOCKING_MODE /**< Enable blocking mode for I2C operations */

const uint8_t I2C_CHANNELS = 2; /**< Number of supported I2C channels */
/**
 * @brief I2C handler configuration.
 *
 * Stores a pointer to an HAL I2C handle, the 7-bit device address and simple
 * timing/retry parameters used by the driver.
 */
typedef struct {
    I2C_HandleTypeDef *hi2c; /**< Pointer to HAL I2C handle */
    uint8_t address;         /**< 7-bit I2C device address */
    uint32_t timeout;        /**< Timeout in ms for I2C operations
                              * @note only used in blocking mode
                              */
    uint8_t retry;           /**< Number of trials for device ready check */
} i2c_handler_t;

/**
 * @brief Initializes the I2C subsystem. Implemented as a weak function in
 * i2c_manager.cc.
 */
void init_i2c(void);

/**
 * @brief Singleton class providing I2C operations.
 *
 * The class provides initialization and minimal primitives required to drive
 * I2C communication. It is implemented as a singleton to simplify global
 * access from embedded code.
 */
class I2CManager {
  public:
    ~I2CManager() = default;

    /**
     * @brief Initialize the I2C manager with given I2C handlers.
     *
     * @param i2c_handlers Array of I2C handlers.
     * @param num_channels Number of I2C handlers in the array.
     */
    static void initialize(i2c_handler_t *i2c_handlers, size_t num_channels);

    /**
     * @brief Get the singleton instance of I2CManager.
     *
     * @return Reference to the I2CManager instance.
     */
    static I2CManager &getInstance();

    /**
     * @brief Subscribe an I2C handler to the manager.
     *
     * @param handler I2C handler configuration to subscribe.
     */
    void subscribeI2CHandler(i2c_handler_t handler);

    /**
     * @brief Unsubscribe an I2C handler from the manager.
     *
     * @param channel Channel index to unsubscribe.
     */
    void unsubscribeI2CHandler(uint8_t channel);

    /**
     * @brief Get I2C handler configuration for a specific channel.
     *
     * @param channel Channel index.
     * @return I2C handler configuration.
     */
    i2c_handler_t getI2CHandler(uint8_t channel);

  private:
    I2CManager() = default;
    I2CManager(const I2CManager &) = delete;
    I2CManager &operator=(const I2CManager &) = delete;

    i2c_handler_t
        i2c_handler[I2C_CHANNELS]; /**< Array of I2C handler configurations */
    bool initialized[I2C_CHANNELS] = {
        false}; /**< Initialization flags for each I2C channel */
};

HAL_StatusTypeDef i2c_write_helper(i2c_handler_t handler, uint8_t *data,
                                   size_t datasize);

HAL_StatusTypeDef i2c_read_helper(i2c_handler_t handler, uint8_t *data,
                                  size_t datasize);
} // namespace i2c
#endif // __I2C_MANAGER_HH__