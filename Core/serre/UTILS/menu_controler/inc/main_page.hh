#ifndef MAIN_PAGE_HH
#define MAIN_PAGE_HH

/**
 * @file main_page.hh
 * @brief Main menu page displaying environmental parameters and controls.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * This header defines the MainPage class which serves as the primary user
 * interface screen displaying temperature, humidity, and PWM status with
 * navigation controls.
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
 * @class MainPage
 * @brief Main menu page for displaying and controlling environmental
 * parameters.
 *
 * Serves as the primary user interface screen displaying current temperature,
 * humidity, and PWM output values. Provides navigation to configuration menus
 * and phase switching between different display modes.
 *
 * @details Displays environmental data in two alternating phases:
 * - DISPLAY_TEMP_HUM: Shows temperature and humidity readings
 * - DISPLAY_PWM: Shows current PWM duty cycle outputs
 * Supports both short and long button presses for different navigation options.
 */
class MainPage : public MenuPage {
  public:
    /**
     * @brief Display the main page with current environmental data.
     *
     * Renders the current display phase (temperature/humidity or PWM status)
     * to the user interface.
     *
     * @return void
     */
    void display() override;

    /**
     * @brief Handle up button short press.
     *
     * Navigates upward in the menu or cycles to previous option.
     *
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Handle down button short press.
     *
     * Navigates downward in the menu or cycles to next option.
     *
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Handle select button short press.
     *
     * Confirms selection and transitions to appropriate submenu or state.
     *
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Handle up button long press.
     *
     * Performs alternative action such as phase switching or menu reset.
     *
     * @return void
     */
    void onLongPressUp() override;

    /**
     * @brief Handle down button long press.
     *
     * Performs alternative action such as phase switching or menu navigation.
     *
     * @return void
     */
    void onLongPressDown() override;

    /**
     * @brief Handle select button long press.
     *
     * Performs extended action such as entering special mode or saving
     * settings.
     *
     * @return void
     */
    void onLongPressSelect() override;

    /**
     * @brief Get the next menu state to transition to.
     *
     * @return MenuState representing the desired state transition.
     */
    inline MenuState getNextState() const override {
        return nextState_;
    }

    /**
     * @brief Switch between display phases.
     *
     * Toggles between temperature/humidity display and PWM output display
     * modes. Allows users to view different environmental parameters.
     *
     * @return void
     */
    inline void switchPhase() {
        this->displayPhase_ =
            (this->displayPhase_ == display_phase_t::DISPLAY_TEMP_HUM)
                ? display_phase_t::DISPLAY_PWM
                : display_phase_t::DISPLAY_TEMP_HUM;
    }

  private:
    display_phase_t displayPhase_ =
        display_phase_t::DISPLAY_TEMP_HUM;  ///< Current display phase (temp/hum
                                            ///< or PWM)
    MenuState nextState_ = MenuState::MAIN; ///< Next menu state for transition
};

} // namespace ui
} // namespace utils

#endif // MAIN_PAGE_HH
