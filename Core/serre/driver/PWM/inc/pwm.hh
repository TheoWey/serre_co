#ifndef PWM_HH
#define PWM_HH

#include "../../../../Inc/tim.h"

/**
 * @file pwm.hh
 * @brief PWM driver interface for STM32 timers.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header declares a PWM wrapper class used to configure and
 * control a single timer channel's duty cycle and output enable state.
 *
 * @details The implementation expects the HAL TIM handle (TIM_HandleTypeDef)
 * to be managed and started elsewhere. This class only stores channel and
 * duty cycle information and provides an API to modify them.
 */

/**
 * @namespace driver
 * @brief Contains classes and methods for various drivers.
 */
namespace driver {
/**
 * @namespace pwm
 * @brief Contains classes and methods for handling PWM actuators.
 */
namespace pwm {

/**
 * @brief Simple PWM handler container.
 *
 * Holds a pointer to the HAL timer handle and the channel ID used by
 * the STM32 HAL library.
 */
typedef struct {
    TIM_HandleTypeDef
        *htim;       ///< Pointer to the HAL timer handle (TIM_HandleTypeDef*)
    uint8_t channel; ///< Timer channel identifier (e.g. TIM_CHANNEL_1)
} pwm_handler_t;

/**
 * @brief PWM control class.
 *
 * Represents a single PWM output channel. Provides methods to set the
 * duty cycle and enable/disable the output.
 *
 * @details The PWM class wraps a single timer channel, caching the period
 * and duty cycle values internally. Output control is performed through
 * the STM32 HAL interface.
 */
class PWM {
  public:
    /**
     * @brief Construct a PWM instance.
     *
     * @param pwm_handler The PWM handler structure containing a pointer to
     *                    the HAL timer handle and the channel identifier.
     *
     * @details The channel value is stored internally. HAL timer configuration
     * and timer start must be performed outside this class. The timer period
     * is cached at construction time.
     * @note The pwm_handler.htim pointer must be valid for the lifetime of
     *       this PWM instance.
     */
    PWM(pwm_handler_t pwm_handler);

    /**
     * @brief Destroy the PWM instance.
     */
    ~PWM() = default;

    /**
     * @brief Set the duty cycle for the PWM output.
     *
     * @param duty_ratio Duty cycle in the range [0, 100].
     *
     * @details Values outside the valid range [0, 100] will be clamped
     * by the implementation to the nearest valid boundary.
     * @return void
     */
    void setDutyCycle(uint8_t duty_ratio);

    /**
     * @brief Get the current duty cycle.
     *
     * @return Current duty cycle value in the range [0, 100].
     */
    inline uint8_t getDutyCycle() const {
        return this->m_duty;
    }

    /**
     * @brief Enable or disable the PWM output.
     *
     * @param on True to enable the output, false to disable it.
     * @return void
     *
     * @details Configures the PWM channel output through the STM32 HAL
     * interface. The duty cycle is preserved when toggling output state.
     */
    void enable(bool on = true);

    /**
     * @brief Disable the PWM output.
     *
     * Convenience method equivalent to enable(false).
     * @return void
     */
    void disable() {
        this->enable(false);
    }

  private:
    pwm_handler_t m_handler; ///< PWM handler structure (timer and channel)
    uint32_t m_period = 0;   ///< Cached timer period value
    uint8_t m_duty = 0;      ///< Current duty cycle value [0..255]
    bool m_enabled = false;  ///< Current output enable state
};

} // namespace pwm
} // namespace driver

#endif // PWM_HH
