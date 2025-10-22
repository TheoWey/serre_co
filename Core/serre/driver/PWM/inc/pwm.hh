#ifndef PWM_HH
#define PWM_HH

#include "../../../../Inc/tim.h"

/**
 * @file pwm.hh
 * @brief PWM driver interface for STM32 timers.
 *
 * This header declares a small PWM wrapper class used to configure and
 * control a single timer channel's duty cycle and output enable state.
 *
 * The implementation expects the HAL TIM handle (TIM_HandleTypeDef) to be
 * managed/started elsewhere; this class only stores channel and duty info
 * and provides an API to change them.
 */

namespace pwm {

/**
 * @brief Simple PWM handler container.
 *
 * Holds a pointer to the HAL timer handle and the channel ID used by HAL.
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
 */
class PWM {
  public:
    /**
     * @brief Construct a PWM instance.
     * @param channel Channel index or identifier (default: 0).
     *
     * The channel value is stored but the HAL timer/compare configuration
     * and timer start must be performed outside this class.
     */
    PWM(pwm_handler_t pwm_handler);

    /**
     * @brief Destroy the PWM instance.
     */
    ~PWM() = default;

    /**
     * @brief Set the duty cycle.
     * @param duty_ratio Duty cycle in the range [0.0, 1.0].
     *
     * Values outside the range will be clamped by the implementation.
     */
    void setDutyCycle(float duty_ratio);

    /**
     * @brief Enable or disable the PWM output.
     * @param on true to enable output, false to disable.
     */
    void enable(bool on = true);

    /**
     * @brief Disable the PWM output.
     */
    void disable() {
        this->enable(false);
    }

  private:
    pwm_handler_t m_handler; ///< PWM handler structure. */
    uint32_t m_period = 0;   ///< Timer period value cached at construction. */
    float m_duty = 0.0f;     ///< Currently set duty ratio [0.0, 1.0]. */
    bool m_enabled = false;  ///< Output enabled state. */
};

} // namespace pwm

#endif // PWM_HH
