#ifndef DEBOUNCER_HH
#define DEBOUNCER_HH

/**
 * @file debouncer.hh
 * @brief Debouncer Header File
 * @author ThéoWey ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 * @details
 * This header file defines the interface for a debouncer utility, including
 * initialization and basic operations. It supports debouncing of digital
 * inputs using STM32 HAL types.
 */

#include "../../../../../Inc/gpio.h"

namespace utils {
namespace debouncer {

const uint8_t DEBOUNCER_CHANNELS = 5; /**< Number of supported debouncer
                                       * channels */
/**
 * @brief Debouncer handler configuration.
 *
 * Stores a pointer to an HAL GPIO handle, the pin mask/number, debounce
 * timing parameters, and internal state.
 */
typedef struct {
    GPIO_TypeDef *port;   /**< Pointer to HAL GPIO port */
    uint16_t pin;         /**< GPIO pin mask/number */
    uint32_t sample_time; /**< Time in ms for stable state sampling */
    uint32_t last_change; /**< Timestamp of the last state change */
    GPIO_PinState state;  /**< Current debounced state */
} DebouncerHandler;

void init_debouncer(void);

/**
 * @brief Initializes the debouncer subsystem. Implemented as a weak function in
 * debouncer.cc.
 */
class Debouncer {
  public:
    /**
     * @brief Constructor for Debouncer class.
     * @param handler Configuration structure for the debouncer.
     */
    inline Debouncer(DebouncerHandler handler) : debouncer_handler(handler) {};

    /**
     * @brief Update the debounced state based on the current GPIO input.
     * @return The current debounced GPIO state.
     */
    void update();

    /**
     * @brief Get the current debounced GPIO state.
     * @return The current debounced GPIO state.
     */
    inline GPIO_PinState getState() const {
        return debouncer_handler.state;
    }

    inline GPIO_TypeDef *getPort() const {
        return debouncer_handler.port;
    }

  private:
    DebouncerHandler debouncer_handler; /**< Active configuration for the
                                         * debouncer */
};

class DebouncerManager {
  public:
    ~DebouncerManager() = default;

    static void initialize(DebouncerHandler *debouncer_handlers,
                           size_t num_channels);

    static DebouncerManager &getInstance();

    inline Debouncer getDebouncer(size_t index) {
        if (index >= DEBOUNCER_CHANNELS ||
            this->debouncers[index].getPort() == nullptr) {
            Error_Handler();
        }
        return this->debouncers[index];
    }

    void updateDebouncer(size_t index);

    void updateAllDebouncers();

  private:
    DebouncerManager() = default;
    DebouncerManager(const DebouncerManager &) = delete;
    DebouncerManager &operator=(const DebouncerManager &) = delete;

    Debouncer debouncers[DEBOUNCER_CHANNELS] = {
        Debouncer(DebouncerHandler({nullptr, 0, 0, 0, GPIO_PIN_RESET})),
        Debouncer(DebouncerHandler({nullptr, 0, 0, 0, GPIO_PIN_RESET})),
        Debouncer(DebouncerHandler({nullptr, 0, 0, 0, GPIO_PIN_RESET})),
        Debouncer(DebouncerHandler({nullptr, 0, 0, 0, GPIO_PIN_RESET})),
        Debouncer(DebouncerHandler({nullptr, 0, 0, 0, GPIO_PIN_RESET}))};
};

} // namespace debouncer
} // namespace utils

#endif // DEBOUNCER_HH
