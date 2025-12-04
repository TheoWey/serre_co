#include "../inc/i2c_manager.hh"

namespace i2c {

__WEAK void init_i2c(void) {
    // Weak implementation for user override
}

void I2CManager::initialize(i2c_handler_t *i2c_handlers, size_t num_channels) {
    I2CManager &instance = getInstance();
    for (size_t i = 0; i < num_channels && i < I2C_CHANNELS; i++) {
        instance.i2c_handler[i] = i2c_handlers[i];
        instance.i2c_handler[i].address = i2c_handlers[i].address << 1;
        instance.initialized[i] = true;
    }
}

I2CManager &I2CManager::getInstance() {
    static I2CManager instance;
    return instance;
}

void I2CManager::subscribeI2CHandler(i2c_handler_t handler) {
    for (size_t i = 0; i < I2C_CHANNELS; i++) {
        if (this->i2c_handler[i].hi2c == nullptr) {
            this->i2c_handler[i] = handler;
            this->initialized[i] = true;
            return;
        } else if (this->i2c_handler[i].hi2c == handler.hi2c) {
            this->i2c_handler[i] = handler;
            this->initialized[i] = true;
            return;
        }
    }
}

void I2CManager::unsubscribeI2CHandler(uint8_t channel) {
    if (channel >= I2C_CHANNELS) {
        Error_Handler();
    }
    this->i2c_handler[channel] = {nullptr, 0, 0, 0};
    this->initialized[channel] = false;
}

i2c_handler_t I2CManager::getI2CHandler(uint8_t channel) {
    if (channel >= I2C_CHANNELS) {
        Error_Handler();
    }
    return this->i2c_handler[channel];
}

#ifdef BLOCKING_MODE

__weak HAL_StatusTypeDef i2c_write_helper(i2c_handler_t handler, uint8_t *data,
                                          size_t datasize) {

    return HAL_I2C_Master_Transmit(handler.hi2c, handler.address, data,
                                   datasize, handler.timeout);
}

__weak HAL_StatusTypeDef i2c_read_helper(i2c_handler_t handler, uint8_t *data,
                                         size_t datasize) {

    return HAL_I2C_Master_Receive(handler.hi2c, handler.address, data, datasize,
                                  handler.timeout);
}

#else

__weak HAL_StatusTypeDef i2c_write_helper(i2c_handler_t handler, uint8_t *data,
                                          size_t datasize) {
    return HAL_I2C_Master_Transmit_DMA(handler.hi2c, handler.address, data,
                                       datasize);
}

__weak HAL_StatusTypeDef i2c_read_helper(i2c_handler_t handler, uint8_t *data,
                                         size_t datasize) {

    return HAL_I2C_Master_Receive_DMA(handler.hi2c, handler.address, data,
                                      datasize);
}

#endif

} // namespace i2c
