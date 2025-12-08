#ifndef PWM_EDIT_PAGE_HH
#define PWM_EDIT_PAGE_HH

#include "menu_page.hh"

namespace utils {
namespace ui {

/**
 * @class PWMEditPage
 * @brief Page d'édition des paramètres PWM (Fan et Pump)
 *
 * Permet de naviguer entre FAN et PUMP, puis d'éditer leurs valeurs.
 */
class PWMEditPage : public EditPage {
  public:
    PWMEditPage() = default;
    virtual ~PWMEditPage() = default;

    void display() override;

    // Implémentations de EditPage
    void onEnterEditMode() override;
    void onExitEditMode() override;
    void adjustValue(int8_t delta) override;
    void navigateSelection(int8_t direction) override;

    // Getter
    inline pwm_channel_t getSelectedChannel() const {
        return selectedChannel_;
    }

  private:
    pwm_channel_t selectedChannel_ = pwm_channel_t::FAN;
};

} // namespace ui
} // namespace utils

#endif // PWM_EDIT_PAGE_HH