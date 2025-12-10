#ifndef SETTINGS_PAGE_HH
#define SETTINGS_PAGE_HH

/**
 * @file settings_page.hh
 * @brief Menu page to navigate system settings.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * Declares the SettingsPage which lists high-level system parameters such as
 * sensors configuration, PWM actuators, and backlight brightness.
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
 * @class SettingsPage
 * @brief Navigable page for system parameters.
 *
 * Allows the user to iterate through top-level settings categories:
 * - PARAMETER_SENSOR: sensor configuration
 * - PARAMETER_ACTUATOR: PWM actuator configuration
 * - PARAMETER_BACKLIGHT_BRIGHTNESS: LCD backlight intensity
 */
class SettingsPage : public NavigablePage {

  public:
    /**
     * @brief Render the settings selection page.
     * @return void
     */
    void display() override;

    /**
     * @brief Move selection up through settings options.
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Move selection down through settings options.
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Confirm the selected settings category.
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Get the currently selected settings parameter.
     * @return Selected display_parameter_t value
     */
    inline display_parameter_t getSelectedParameter() const {
        return selParameter_;
    }

  private:
    display_parameter_t selParameter_ =
        display_parameter_t::PARAMETER_SENSOR; ///< Current selection
};

} // namespace ui
} // namespace utils

#endif // SETTINGS_PAGE_HH
