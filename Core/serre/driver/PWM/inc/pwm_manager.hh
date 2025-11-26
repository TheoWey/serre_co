#ifndef PWM_MANAGER_HH
#define PWM_MANAGER_HH

#include "pwm.hh"

/**
 * @file pwm_manager.hh
 * @brief PWM manager interface for handling multiple PWM channels.
 *
 * This header declares a PWMManager class that manages multiple PWM
 * instances. It provides methods to initialize, set duty cycles, and
 * enable/disable all PWM channels collectively.
 */

namespace pwm {
/**
 * @brief PWM Manager class.
 *
 * Manages multiple PWM instances, allowing collective control over
 * their duty cycles and enable states.
 */

constexpr size_t MAX_PWM_CHANNELS = 10;

class PWMManager {
  public:
    /**
     * @brief Get the singleton instance of PWMManager.
     * @return Reference to the PWMManager instance.
     */
    static PWMManager &getInstance();

    /**
     * @brief Initialize the PWM manager with given PWM handlers.
     * @param pwm_handlers Array of PWM handlers.
     * @param count Number of PWM handlers in the array.
     */
    static void initialize(pwm_handler_t *pwm_handlers, size_t count);

    /**
     * @brief Subscribe a PWM instance to the manager.
     * @param pwm Pointer to the PWM instance to subscribe.
     */
    void subscribePWM(PWM *pwm);

    /**
     * @brief Set duty cycle for all managed PWM channels.
     * @param duty_ratio Duty cycle in the range [0.0, 1.0].
     */
    void setAllDutyCycles(float duty_ratio);

    /**
     * @brief Set duty cycle for a specific PWM channel.
     * @param index Index of the PWM channel.
     * @param duty_ratio Duty cycle in the range [0.0, 1.0].
     */
    void setDutyCycle(size_t index, float duty_ratio);

    /**
     * @brief Enable or disable all managed PWM channels.
     * @param on True to enable, false to disable.
     */
    void enableAll(bool on);

    /**
     * @brief Enable or disable a specific PWM channel.
     * @param index Index of the PWM channel.
     * @param on True to enable, false to disable.
     */
    void enable(size_t index, bool on);

    ~PWMManager();

  private:
    PWMManager();

    PWMManager(const PWMManager &) = delete;
    PWMManager &operator=(const PWMManager &) = delete;

    PWM *pwms[MAX_PWM_CHANNELS] = {nullptr}; ///< Array of PWM pointers
};

} // namespace pwm
#endif // PWM_MANAGER_HH