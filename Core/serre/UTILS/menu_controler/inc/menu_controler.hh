#ifndef MENU_CONTROLER_HH
#define MENU_CONTROLER_HH

/**
 * @file menu_controler.hh
 * @brief User interface controller managing menu states and button input.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the UIControler singleton which manages the user
 * interface state machine, coordinates menu pages, handles button input, and
 * controls display updates for the greenhouse control system.
 */

#include "hysteresis_settings.hh"
#include "main_page.hh"
#include "menu_page.hh"
#include "pwm_edit_page.hh"
#include "sensors_parameter.hh"
#include "settings_page.hh"

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
 * @enum EditMode
 * @brief User interface modes for interaction.
 *
 * Defines the current interaction mode: navigating through menus or editing
 * values.
 */
enum class EditMode : uint8_t {
    NAVIGATING, ///< Navigation mode - browsing menus
    EDITING     ///< Editing mode - modifying values
};

/**
 * @enum ControllerState
 * @brief Overall controller states.
 *
 * Defines the high-level states of the user interface controller.
 */
enum class ControllerState : uint8_t {
    WELCOME_SCREEN, ///< Displaying welcome/startup screen
    NORMAL_MODE     ///< Normal menu operation
};

class UIControler {
  public:
    /**
     * @brief Get the singleton instance of UIControler.
     *
     * @return Reference to the UIControler instance.
     */
    static UIControler &getInstance();

    /**
     * @brief Initialize the UI controller.
     *
     * @param showWelcomeScreen Whether to display welcome screen on startup.
     * @param switchPeriodMs Period in milliseconds for automatic page
     * switching.
     * @return void
     */
    void init(bool showWelcomeScreen = true, uint32_t switchPeriodMs = 5000);

    /**
     * @brief Update UI controller state and display.
     *
     * Polls buttons, handles input, updates state machine, and refreshes
     * display. Should be called periodically from the main application loop.
     *
     * @return void
     */
    void update();

    /**
     * @brief Set the value adjustment delta.
     *
     * Sets the step size for value adjustments during editing.
     *
     * @param delta The adjustment step size.
     * @return void
     */
    inline void setDelta_m(uint8_t delta) {
        this->delta_m = delta;
    }

    /**
     * @brief Set the current menu page.
     *
     * Transitions to the specified menu page.
     *
     * @param menuType The MenuState to transition to.
     * @return void
     */
    inline void setCurrentMenu(MenuState menuType) {
        switch (menuType) {
        case MenuState::MAIN:
            this->switchToPage_(MenuState::MAIN);
            break;
        case MenuState::SETTINGS:
            this->switchToPage_(MenuState::SETTINGS);
            break;
        default:
            break;
        }
    }
    /**
     * @brief Get the timestamp of the last user action.
     *
     * Used for timeout calculations and idle detection.
     *
     * @return Timestamp in milliseconds of the last button action.
     */
    inline uint32_t getLastActionTick() const {
        return this->lastActionTick_;
    }

  private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    UIControler();
    UIControler(const UIControler &) = delete;
    UIControler &operator=(const UIControler &) = delete;

    MenuPage *currentPage_;     ///< Pointer to currently displayed page
    MainPage mainPage_;         ///< Main display page
    SettingsPage settingsPage_; ///< Settings menu page
    PWMEditPage pwmEditPage_;   ///< PWM editing page
    SensorTypeSelectPage sensorTypeSelectPage_;     ///< Sensor type selection
    SensorNumberSelectPage sensorNumberSelectPage_; ///< Sensor number selection
    TempOffsetEditPage tempOffsetEditPage_; ///< Temperature offset editor
    HumidityCalibEditPage
        humidityCalibEditPage_; ///< Humidity calibration editor
    SetpointTypeSelectPage setpointTypeSelectPage_; ///< Setpoint type selector
    SetpointModeSelectPage setpointModeSelectPage_; ///< Setpoint mode selector
    SetpointValueEditPage setpointValueEditPage_;   ///< Setpoint value editor

    MenuState currentState_;          ///< Current menu state
    EditMode editMode_;               ///< Current edit/navigate mode
    ControllerState controllerState_; ///< Overall controller state

    uint32_t lastSwitchTick_;    ///< Timestamp of last page switch
    uint32_t switchPeriodMs_;    ///< Period for automatic page switching
    uint32_t welcomeDurationMs_; ///< Welcome screen display duration
    uint32_t lastActionTick_;    ///< Timestamp of last user action
    uint8_t delta_m;             ///< Value adjustment step size

    /**
     * @brief Poll button input states.
     * @return void
     */
    void pollButtons_();

    /**
     * @brief Handle up button press.
     * @return void
     */
    void handleButtonUp_();

    /**
     * @brief Handle down button press.
     * @return void
     */
    void handleButtonDown_();

    /**
     * @brief Handle select button press.
     * @return void
     */
    void handleButtonSelect_();

    /**
     * @brief Handle select button long press.
     * @return void
     */
    void handleLongPressSelect_();

    /**
     * @brief Switch to a specific menu page.
     *
     * @param newState The MenuState to switch to.
     * @return void
     */
    void switchToPage_(MenuState newState);

    /**
     * @brief Display the current menu page.
     * @return void
     */
    void displayCurrentPage_();

    /**
     * @brief Show the welcome screen.
     * @return void
     */
    void showWelcomeScreen();

    /**
     * @brief Get the current edit page if in editing mode.
     *
     * @return Pointer to the current EditPage or nullptr if not in edit mode.
     */
    EditPage *getCurrentEditPage_() const;
};

} // namespace ui
} // namespace utils

#endif // MENU_CONTROLER_HH