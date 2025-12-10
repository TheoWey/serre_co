#ifndef PWM_EDIT_PAGE_HH
#define PWM_EDIT_PAGE_HH

/**
 * @file pwm_edit_page.hh
 * @brief Menu page for editing PWM parameters (Fan and Pump).
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * Defines the PWMEditPage class used to navigate between FAN and PUMP PWM
 * channels and edit their duty cycle values.
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
 * @class PWMEditPage
 * @brief Menu page for editing PWM channels (Fan and Pump).
 *
 * Provides navigation between PWM channels and allows adjusting their duty
 * cycle values. Integrates with the EditPage base class for edit-mode
 * handling.
 */
class PWMEditPage : public EditPage {
  public:
    /**
     * @brief Default constructor.
     */
    PWMEditPage() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~PWMEditPage() = default;

    /**
     * @brief Render the PWM edit page to the display.
     * @return void
     */
    void display() override;

    /**
     * @brief Enter edit mode for PWM value adjustment.
     * @return void
     */
    void onEnterEditMode() override;

    /**
     * @brief Exit edit mode after adjustments.
     * @return void
     */
    void onExitEditMode() override;

    /**
     * @brief Adjust the PWM duty cycle value by delta.
     *
     * @param delta Increment/decrement amount (positive or negative).
     * @return void
     */
    void adjustValue(int8_t delta) override;

    /**
     * @brief Navigate between selectable PWM channels.
     *
     * @param direction Navigation direction (positive or negative).
     * @return void
     */
    void navigateSelection(int8_t direction) override;

    /**
     * @brief Get the currently selected PWM channel.
     *
     * @return Selected pwm_channel_t (FAN or PUMP).
     */
    inline pwm_channel_t getSelectedChannel() const {
        return selectedChannel_;
    }

  private:
    pwm_channel_t selectedChannel_ =
        pwm_channel_t::FAN; ///< Currently selected PWM channel
};

} // namespace ui
} // namespace utils

#endif // PWM_EDIT_PAGE_HH