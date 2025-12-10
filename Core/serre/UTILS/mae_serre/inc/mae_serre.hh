#ifndef MAE_SERRE_HH
#define MAE_SERRE_HH

/**
 * @file mae_serre.hh
 * @brief State machine and control loop for greenhouse environment management.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the SerreController singleton which implements the
 * finite state machine (mae_serre) for controlling greenhouse operations
 * including temperature and humidity regulation.
 */

#include "stdint.h"

/**
 * @namespace utils
 * @brief Contains utility classes and methods.
 */
namespace utils {
/**
 * @namespace mae_serre
 * @brief Contains classes and methods for greenhouse environment management.
 */
namespace mae_serre {

/**
 * @enum SerreMode
 * @brief Operating modes for the greenhouse control system.
 *
 * Defines the available states in the finite state machine for greenhouse
 * environment management.
 */
enum class SerreMode : uint8_t {
    NORMAL,     ///< Normal operation with automatic environmental control
    SETTING,    ///< Configuration/adjustment mode for setpoints and parameters
    STANDBY,    ///< Standby mode with minimal operation
    ERROR,      ///< Error state triggered by sensor or system failure
    MAINTENANCE ///< Maintenance mode for service and testing
};

/**
 * @class SerreController
 * @brief Finite state machine and control loop manager for greenhouse
 * operations.
 *
 * Implements a singleton pattern to manage the greenhouse environment by
 * coordinating sensor readings, PWM output updates, and state transitions.
 * Maintains temperature and humidity setpoints with automatic control logic.
 *
 * @details The controller runs periodic update loops for sensor data processing
 * and PWM control with configurable timeouts. It manages five operational
 * states: NORMAL (automatic control), SETTING (parameter adjustment), STANDBY
 * (idle), ERROR (fault condition), and MAINTENANCE (service mode).
 */
class SerreController {
  public:
    /**
     * @brief Get the singleton instance of SerreController.
     *
     * @return Reference to the SerreController instance.
     */
    static SerreController &getInstance();

    /**
     * @brief Main control loop process.
     *
     * Orchestrates all state machine logic, sensor updates, and PWM control.
     * Should be called periodically from the main application loop.
     *
     * @return void
     */
    void processLoop();

    /**
     * @brief PWM control loop update.
     *
     * Updates PWM outputs based on current mode and environmental setpoints.
     * Called periodically at intervals defined by pwmUpdatePeriodMs_.
     *
     * @return void
     */
    void pwm_control_loop(void);

    /**
     * @brief Sensor data update loop.
     *
     * Reads and processes sensor data from all registered sensors.
     * Called periodically at intervals defined by sensorUpdatePeriodMs_.
     *
     * @return void
     */
    void sensor_update_loop(void);

    /**
     * @brief Get the current operating mode.
     *
     * @return Current SerreMode state.
     */
    inline SerreMode getCurrentMode() const {
        return currentMode_;
    }

    /**
     * @brief Set the operating mode.
     *
     * Transitions the state machine to a new mode.
     *
     * @param newMode The SerreMode to transition to.
     * @return void
     */
    inline void setMode(SerreMode newMode) {
        currentMode_ = newMode;
    };

    /**
     * @brief Set the temperature setpoint (activation threshold).
     *
     * Sets the temperature at which heating/cooling activates.
     *
     * @param setpoint Temperature setpoint in appropriate units (typically °C).
     * @return void
     */
    inline void setTemperatureSetpoint(uint16_t setpoint) {
        temperatureSetpoint_ = setpoint;
    }

    /**
     * @brief Set the temperature reset point (deactivation threshold).
     *
     * Sets the temperature at which heating/cooling deactivates.
     *
     * @param resetPoint Temperature reset point in appropriate units (typically
     * °C).
     * @return void
     */
    inline void setTemperatureResetPoint(uint16_t resetPoint) {
        temperatureResetPoint_ = resetPoint;
    }

    /**
     * @brief Set the humidity setpoint (activation threshold).
     *
     * Sets the humidity level at which humidification/dehumidification
     * activates.
     *
     * @param setpoint Humidity setpoint as a percentage [0, 100].
     * @return void
     */
    inline void setHumiditySetpoint(uint16_t setpoint) {
        humiditySetpoint_ = setpoint;
    }

    /**
     * @brief Set the humidity reset point (deactivation threshold).
     *
     * Sets the humidity level at which humidification/dehumidification
     * deactivates.
     *
     * @param resetPoint Humidity reset point as a percentage [0, 100].
     * @return void
     */
    inline void setHumidityResetPoint(uint16_t resetPoint) {
        humidityResetPoint_ = resetPoint;
    }

    /**
     * @brief Get the temperature setpoint.
     *
     * @return Current temperature setpoint.
     */
    inline uint16_t getTemperatureSetpoint() const {
        return temperatureSetpoint_;
    }

    /**
     * @brief Get the temperature reset point.
     *
     * @return Current temperature reset point.
     */
    inline uint16_t getTemperatureResetPoint() const {
        return temperatureResetPoint_;
    }

    /**
     * @brief Get the humidity setpoint.
     *
     * @return Current humidity setpoint percentage.
     */
    inline uint16_t getHumiditySetpoint() const {
        return humiditySetpoint_;
    }

    /**
     * @brief Get the humidity reset point.
     *
     * @return Current humidity reset point percentage.
     */
    inline uint16_t getHumidityResetPoint() const {
        return humidityResetPoint_;
    }

  private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    SerreController() : currentMode_(SerreMode::NORMAL) {
    }
    SerreController(const SerreController &) = delete;
    SerreController &operator=(const SerreController &) = delete;

    SerreMode currentMode_;             ///< Current finite state machine mode
    uint32_t lastSensorUpdateTick_ = 0; ///< Timestamp of last sensor update
    uint32_t lastPwmUpdateTick_ = 0;    ///< Timestamp of last PWM update
    uint32_t sensorUpdatePeriodMs_ =
        2000; ///< Sensor update period in milliseconds
    uint32_t pwmUpdatePeriodMs_ = 3000; ///< PWM update period in milliseconds
    uint32_t lastSettingModeTick_ =
        0; ///< Timestamp of last SETTING mode activity
    uint32_t settingModeTimeoutMs_ =
        60000; ///< Timeout duration for SETTING mode

    uint16_t temperatureSetpoint_ = 25; ///< Temperature activation threshold
    uint16_t temperatureResetPoint_ =
        20;                            ///< Temperature deactivation threshold
    uint16_t humiditySetpoint_ = 50;   ///< Humidity activation threshold (%)
    uint16_t humidityResetPoint_ = 60; ///< Humidity deactivation threshold (%)
};

/**
 * @brief PWM control loop update function.
 *
 * Convenience function that delegates to
 * SerreController::getInstance().pwm_control_loop().
 *
 * @return void
 */
void pwm_control_loop(void);

/**
 * @brief Sensor data update loop function.
 *
 * Convenience function that delegates to
 * SerreController::getInstance().sensor_update_loop().
 *
 * @return void
 */
void sensor_update_loop(void);

} // namespace mae_serre
} // namespace utils

#endif // MAE_SERRE_HH
