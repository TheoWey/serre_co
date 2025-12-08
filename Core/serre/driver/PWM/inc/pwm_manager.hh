#ifndef PWM_MANAGER_HH
#define PWM_MANAGER_HH

#include "pwm.hh"

/**
 * @file pwm_manager.hh
 * @brief PWM manager interface for handling multiple PWM channels.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.1
 *
 * This header declares a PWMManager class that manages multiple PWM
 * instances. It provides methods to initialize, set duty cycles, and
 * enable/disable all PWM channels collectively.
 */

namespace driver {
namespace pwm {

/**
 * @brief PWM Manager class.
 *
 * Manages multiple PWM instances, allowing collective control over
 * their duty cycles and enable states.
 */

constexpr size_t MAX_PWM_CHANNELS = 10;

/**
 * Initialize the PWM subsystem.
 * @brief Initializes the PWM subsystem. implemented as a weak function in
 * pwm_manager.cc.
 */
void init_pwm(void);

class PWMManager {
  public:
    /**
     * @brief Initialize the PWM manager with given PWM handlers.
     *
     * Copies up to MAX_PWM_CHANNELS entries from the provided array into the
     * manager and marks those channels as initialized.
     *
     * @param pwm_handlers Pointer to an array of pwm_handler_t to register.
     *                     May be nullptr if num_channels is 0.
     * @param num_channels Number of entries in pwm_handlers. Only the first
     *                     MIN(num_channels, MAX_PWM_CHANNELS) entries are used.
     *
     * @note Calling initialize more than once will overwrite the previously
     *       registered handlers and reinitialize the manager state.
     * @note Must be called before using getInstance() or any PWM operations.
     */
    static void initialize(pwm_handler_t *pwm_handlers, size_t num_channels);

    /**
     * @brief Get the singleton instance of PWMManager.
     *
     * @return Reference to the PWMManager instance.
     *
     * @note initialize() must be called before first use of getInstance().
     */
    static PWMManager &getInstance();

    /**
     * @brief Register (subscribe) a PWM handler with the manager.
     *
     * The handler is stored in the first available slot. If all slots are
     * occupied, this call has no effect.
     *
     * @param handler The pwm_handler_t instance to register (copied).
     *
     * @note Does not allocate memory; simply stores the handler in the
     *       internal array and updates bookkeeping (initialized/active).
     */
    void subscribePWM(PWM pwm_instance);

    /**
     * @brief Unregister (unsubscribe) a PWM handler by index.
     *
     * Marks the slot at the given index as uninitialized and decrements
     * the active channel count. If the index is out of range or the slot
     * is already uninitialized, this call has no effect.
     *
     * @param index Index of the PWM channel to remove (0..MAX_PWM_CHANNELS-1).
     */
    void unsubscribePWM(size_t index);

    /**
     * @brief Set duty cycle for all managed PWM channels.
     *
     * @param duty_ratio Duty cycle in the range [0.0, 1.0].
     */
    void setAllDutyCycles(float duty_ratio);

    /**
     * @brief Set duty cycle for a specific PWM channel.
     *
     * @param index Index of the PWM channel.
     * @param duty_ratio Duty cycle in the range [0.0, 1.0].
     */
    void setDutyCycle(size_t index, float duty_ratio);

    /**
     * @brief Enable or disable all managed PWM channels.
     *
     * @param on True to enable, false to disable.
     */
    void enableAll(bool on);

    /**
     * @brief Enable or disable a specific PWM channel.
     *
     * @param index Index of the PWM channel.
     * @param on True to enable, false to disable.
     */
    void enable(size_t index, bool on);

    /**
     * @brief Get pointer to a PWM instance by index.
     *
     * @param index Index of the PWM channel.
     * @return Pointer to PWM instance or nullptr if invalid index.
     */
    PWM *getPWM(size_t index);

    ~PWMManager() = default;

  private:
    PWMManager() = default;
    PWMManager(const PWMManager &) = delete;
    PWMManager &operator=(const PWMManager &) = delete;

    PWM pwm_instances[MAX_PWM_CHANNELS] = {
        PWM({nullptr, 0}), PWM({nullptr, 0}), PWM({nullptr, 0}),
        PWM({nullptr, 0}), PWM({nullptr, 0}), PWM({nullptr, 0}),
        PWM({nullptr, 0}), PWM({nullptr, 0}), PWM({nullptr, 0}),
        PWM({nullptr, 0})}; ///< Preallocated PWM instances

    bool initialized[MAX_PWM_CHANNELS] = {
        false, false, false, false, false,
        false, false, false, false, false}; ///< Track initialized channels

    size_t active_channels_ = 0; ///< Number of active PWM channels
};

} // namespace pwm
} // namespace driver

#endif // PWM_MANAGER_HH
