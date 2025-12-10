#ifndef HYSTERESIS_SETTINGS_HH
#define HYSTERESIS_SETTINGS_HH

/**
 * @file hysteresis_settings.hh
 * @brief User interface pages for configuring hysteresis (setpoint) parameters.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines menu pages for configuring temperature and humidity
 * setpoints and reset points with hysteresis control logic.
 */

#include "menu_page.hh"

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
 * @enum SetpointType
 * @brief Types of environmental parameters that have setpoints.
 *
 * Identifies which environmental parameter (temperature or humidity) is being
 * configured in the user interface.
 */
enum class SetpointType {
    TEMPERATURE, ///< Temperature setpoint configuration
    HUMIDITY     ///< Humidity setpoint configuration
};
/**
 * @enum SetpointMode
 * @brief Hysteresis control modes for setpoints.
 *
 * Distinguishes between the activation setpoint and deactivation (reset) point
 * in the hysteresis control scheme.
 */
enum class SetpointMode {
    SETPOINT,   ///< Activation threshold (trigger point)
    RESET_POINT ///< Deactivation threshold (reset/hysteresis point)
};

/**
 * @class SetpointTypeSelectPage
 * @brief Menu page for selecting which parameter type (temperature/humidity) to
 * configure.
 *
 * Displays a navigable menu allowing users to select between temperature and
 * humidity setpoint configuration. Uses up/down buttons for navigation and
 * select button to confirm choice.
 */
class SetpointTypeSelectPage : public NavigablePage {
  public:
    /**
     * @brief Default constructor for SetpointTypeSelectPage.
     */
    SetpointTypeSelectPage() = default;

    /**
     * @brief Display the setpoint type selection menu.
     * @return void
     */
    void display() override;

    /**
     * @brief Handle up button press (navigate to previous selection).
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Handle down button press (navigate to next selection).
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Handle select button press (confirm selection).
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Get the currently selected setpoint type.
     *
     * @return Selected SetpointType (TEMPERATURE or HUMIDITY).
     */
    inline SetpointType getSelectedType() const {
        return selectedType_;
    }
    inline MenuState getNextState() const override {
        return MenuState::SETPOINT_MODE_SELECT;
    }

  private:
    SetpointType selectedType_ = SetpointType::TEMPERATURE;
};

/**
 * @class SetpointModeSelectPage
 * @brief Menu page for selecting which hysteresis mode (setpoint/reset) to
 * configure.
 *
 * Displays a navigable menu allowing users to select between setpoint
 * (activation threshold) and reset point (deactivation threshold) configuration
 * for the previously selected parameter type.
 */
class SetpointModeSelectPage : public NavigablePage {
  public:
    /**
     * @brief Default constructor for SetpointModeSelectPage.
     */
    SetpointModeSelectPage() = default;

    /**
     * @brief Set the parameter type for this mode selection.
     *
     * @param type The SetpointType (TEMPERATURE or HUMIDITY) being configured.
     * @return void
     */
    inline void setType(SetpointType type) {
        selectedType_ = type;
        selectedMode_ = SetpointMode::SETPOINT;
    }

    /**
     * @brief Display the setpoint mode selection menu.
     * @return void
     */
    void display() override;

    /**
     * @brief Handle up button press (navigate to previous mode selection).
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Handle down button press (navigate to next mode selection).
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Handle select button press (confirm mode selection).
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Get the parameter type.
     *
     * @return The SetpointType being configured.
     */
    inline SetpointType getSelectedType() const {
        return selectedType_;
    }

    /**
     * @brief Get the currently selected mode.
     *
     * @return Selected SetpointMode (SETPOINT or RESET_POINT).
     */
    inline SetpointMode getSelectedMode() const {
        return selectedMode_;
    }

    /**
     * @brief Get the next menu state after mode selection.
     *
     * @return MenuState::SETPOINT_VALUE_EDIT for editing the selected setpoint
     * value.
     */
    inline MenuState getNextState() const override {
        return MenuState::SETPOINT_VALUE_EDIT;
    }

  private:
    SetpointType selectedType_ = SetpointType::TEMPERATURE;
    SetpointMode selectedMode_ = SetpointMode::SETPOINT;
};

/**
 * @class SetpointValueEditPage
 * @brief Menu page for editing the numerical value of a setpoint or reset
 * point.
 *
 * Provides an editable interface where users can adjust temperature or humidity
 * setpoint values using increment/decrement controls. Displays the current
 * value and allows entry mode for precise value editing.
 */
class SetpointValueEditPage : public EditPage {
  public:
    /**
     * @brief Default constructor for SetpointValueEditPage.
     */
    SetpointValueEditPage() = default;

    /**
     * @brief Configure which sensor parameter and mode to edit.
     *
     * @param type The SetpointType (TEMPERATURE or HUMIDITY) being edited.
     * @param mode The SetpointMode (SETPOINT or RESET_POINT) being edited.
     * @return void
     */
    void setSensor(SetpointType type, SetpointMode mode);

    /**
     * @brief Display the setpoint value editor.
     * @return void
     */
    void display() override;

    /**
     * @brief Handle entry into value edit mode.
     * @return void
     */
    void onEnterEditMode() override;

    /**
     * @brief Handle exit from value edit mode.
     * @return void
     */
    void onExitEditMode() override;

    /**
     * @brief Adjust the value by the specified delta.
     *
     * @param delta Increment/decrement amount (positive or negative).
     * @return void
     */
    void adjustValue(int8_t delta) override;

    /**
     * @brief Handle navigation selection (not used for value editing).
     *
     * @param direction Direction of navigation.
     * @return void
     */
    void navigateSelection(int8_t direction) override {
    }

  private:
    SetpointType selectedType_ =
        SetpointType::TEMPERATURE; ///< Selected parameter type
    SetpointMode selectedMode_ =
        SetpointMode::SETPOINT; ///< Selected hysteresis mode
    uint16_t temperatureSetpoint_ =
        25; ///< Temperature activation threshold (°C)
    uint16_t temperatureResetPoint_ =
        30; ///< Temperature deactivation threshold (°C)
    uint16_t humiditySetpoint_ = 50;   ///< Humidity activation threshold (%)
    uint16_t humidityResetPoint_ = 60; ///< Humidity deactivation threshold (%)
};

} // namespace ui
} // namespace utils

#endif // SETPOINT_SETTINGS_HH
