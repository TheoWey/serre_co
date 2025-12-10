#ifndef MENU_PAGE_HH
#define MENU_PAGE_HH

/**
 * @file menu_page.hh
 * @brief Base classes and enumerations for menu page interface framework.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the abstract base classes and enumeration types for
 * the user interface menu system, including page types, menu states, and
 * display modes.
 */

#include <stdint.h>

/**
 * @enum hum_channel_t
 * @brief Humidity sensor channel identifiers.
 */
enum hum_channel_t {
    SOIL_HUMIDITY_0, ///< Soil humidity sensor 0
    SOIL_HUMIDITY_1, ///< Soil humidity sensor 1
    SOIL_HUMIDITY_2, ///< Soil humidity sensor 2
    SOIL_HUMIDITY_3, ///< Soil humidity sensor 3
    SOIL_HUMIDITY_4  ///< Soil humidity sensor 4
};

/**
 * @enum temp_channel_t
 * @brief Temperature sensor channel identifiers.
 */
enum temp_channel_t {
    TEMPERATURE_0, ///< Temperature sensor 0
    TEMPERATURE_1, ///< Temperature sensor 1
    TEMPERATURE_2, ///< Temperature sensor 2
    TEMPERATURE_3, ///< Temperature sensor 3
    TEMPERATURE_4  ///< Temperature sensor 4
};

/**
 * @enum i2c_channel_t
 * @brief I2C communication channel identifiers.
 */
enum i2c_channel_t {
    I2C_CHANNEL_0, ///< I2C channel 0
    I2C_CHANNEL_1  ///< I2C channel 1
};

/**
 * @enum pwm_channel_t
 * @brief PWM actuator channel identifiers.
 */
enum pwm_channel_t {
    FAN, ///< Fan actuator control
    PUMP ///< Pump actuator control
};

/**
 * @enum bebouncer_channel_t
 * @brief Button input channel identifiers for debouncing.
 */
enum bebouncer_channel_t {
    BUTTON_UP,    ///< Up button input
    BUTTON_DOWN,  ///< Down button input
    BUTTON_SELECT ///< Select button input
};

/**
 * @enum HysteresisParameter
 * @brief Parameters that use hysteresis control.
 */
enum class HysteresisParameter : uint8_t {
    TEMPERATURE_HYSTERESIS, ///< Temperature control with hysteresis
    HUMIDITY_HYSTERESIS     ///< Humidity control with hysteresis
};

/**
 * @namespace utils
 * @brief Contains utility classes and methods.
 */
namespace utils {
/**
 * @namespace ui
 * @brief Contains classes and methods for user interface management.
 */
namespace ui {

/**
 * @enum MenuState
 * @brief Enumeration of all possible menu states in the UI state machine.
 */
enum class MenuState {
    MAIN,                 ///< Main display page
    SETTINGS,             ///< Settings menu page
    PWM_EDIT,             ///< PWM duty cycle editor
    SENSORS_PARAMETER,    ///< Sensor parameter selection
    SENSOR_NUMBER_SELECT, ///< Sensor number selection
    TEMP_OFFSET_EDIT,     ///< Temperature offset calibration editor
    HUMIDITY_CALIB_EDIT,  ///< Humidity calibration editor
    SETPOINT_TYPE_SELECT, ///< Setpoint type selector (temp/humidity)
    SETPOINT_MODE_SELECT, ///< Setpoint mode selector (setpoint/reset)
    SETPOINT_VALUE_EDIT   ///< Setpoint value editor
};

/**
 * @enum display_phase_t
 * @brief Display phase modes for main page alternation.
 */
enum class display_phase_t {
    DISPLAY_TEMP_HUM, ///< Display temperature and humidity values
    DISPLAY_PWM       ///< Display PWM duty cycle outputs
};

/**
 * @enum display_parameter_t
 * @brief Types of parameters that can be displayed.
 */
enum class display_parameter_t {
    PARAMETER_SENSOR,   ///< Sensor reading parameters
    PARAMETER_ACTUATOR, ///< Actuator control parameters
    PARAMETER_SETPOINT  ///< Setpoint configuration parameters
};

/**
 * @class MenuPage
 * @brief Abstract base class for all menu pages.
 *
 * Defines the interface for menu pages including display rendering and button
 * input handling. All menu pages must inherit from this class and implement
 * the pure virtual methods.
 *
 * @details Supports both short and long button presses, display updates, and
 * state transitions. Each page tracks its display phase and reports its next
 * state for menu navigation.
 */
class MenuPage {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~MenuPage() = default;

    /**
     * @brief Render the page content to the display.
     * @return void
     */
    virtual void display() = 0;

    /**
     * @brief Handle up button short press.
     * @return void
     */
    virtual void onButtonUp() = 0;

    /**
     * @brief Handle down button short press.
     * @return void
     */
    virtual void onButtonDown() = 0;

    /**
     * @brief Handle select button short press.
     * @return void
     */
    virtual void onButtonSelect() = 0;

    /**
     * @brief Handle up button long press.
     * @return void
     */
    virtual void onLongPressUp() = 0;

    /**
     * @brief Handle down button long press.
     * @return void
     */
    virtual void onLongPressDown() = 0;

    /**
     * @brief Handle select button long press.
     * @return void
     */
    virtual void onLongPressSelect() = 0;

    /**
     * @brief Get the next menu state after this page.
     *
     * @return The MenuState to transition to after this page.
     */
    virtual MenuState getNextState() const {
        return MenuState::MAIN;
    }

  protected:
    display_phase_t displayPhase_ = display_phase_t::DISPLAY_TEMP_HUM;
};

/**
 * @class NavigablePage
 * @brief Base class for navigable menu pages.
 *
 * Extends MenuPage with default implementations for long-press handlers.
 * Provides a base for pages that primarily navigate between options using
 * up/down buttons without requiring long-press functionality.
 *
 * @details Long-press handlers are empty stubs; subclasses can override
 * if needed. Returns to MAIN state by default.
 */
class NavigablePage : public MenuPage {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~NavigablePage() = default;

    /**
     * @brief Handle up button long press (default: empty).
     * @return void
     */
    void onLongPressUp() override {
    }

    /**
     * @brief Handle down button long press (default: empty).
     * @return void
     */
    void onLongPressDown() override {
    }

    /**
     * @brief Handle select button long press (default: empty).
     * @return void
     */
    void onLongPressSelect() override {
    }

    /**
     * @brief Get the next menu state (default: MAIN).
     *
     * @return MenuState::MAIN as the default next state.
     */
    MenuState getNextState() const override {
        return MenuState::MAIN;
    }

  protected:
    NavigablePage() = default;
};

/**
 * @class EditPage
 * @brief Base class for pages that allow value editing.
 *
 * Extends MenuPage with functionality for entering/exiting edit mode and
 * adjusting values. Provides default implementations for button handlers that
 * map to value adjustment and selection navigation.
 *
 * @details Up/down buttons navigate between selectable items, and subclasses
 * can override to adjust values in edit mode. Returns to SETTINGS state by
 * default for menu navigation.
 */
class EditPage : public MenuPage {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~EditPage() = default;

    /**
     * @brief Handle entry into value edit mode.
     * @return void
     */
    virtual void onEnterEditMode() = 0;

    /**
     * @brief Handle exit from value edit mode.
     * @return void
     */
    virtual void onExitEditMode() = 0;

    /**
     * @brief Adjust the current value by delta.
     *
     * @param delta Amount to adjust value (positive or negative).
     * @return void
     */
    virtual void adjustValue(int8_t delta) = 0;

    /**
     * @brief Navigate selection between editable items.
     *
     * @param direction Navigation direction (positive or negative).
     * @return void
     */
    virtual void navigateSelection(int8_t direction) = 0;

    /**
     * @brief Handle up button short press (navigate selection left).
     * @return void
     */
    void onButtonUp() override {
        navigateSelection(-1);
    }

    /**
     * @brief Handle down button short press (navigate selection right).
     * @return void
     */
    void onButtonDown() override {
        navigateSelection(1);
    }

    /**
     * @brief Handle select button short press (enter/exit edit mode).
     * @return void
     */
    void onButtonSelect() override {
    }

    /**
     * @brief Handle up button long press (default: empty).
     * @return void
     */
    void onLongPressUp() override {
    }

    /**
     * @brief Handle down button long press (default: empty).
     * @return void
     */
    void onLongPressDown() override {
    }

    /**
     * @brief Handle select button long press (default: empty).
     * @return void
     */
    void onLongPressSelect() override {
    }

    /**
     * @brief Get the next menu state (default: SETTINGS).
     *
     * @return MenuState::SETTINGS as the default next state.
     */
    MenuState getNextState() const override {
        return MenuState::SETTINGS;
    }

  protected:
    EditPage() = default;
};

} // namespace ui
} // namespace utils

#endif // MENU_PAGE_HH
