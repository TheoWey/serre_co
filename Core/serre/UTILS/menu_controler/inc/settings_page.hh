#ifndef SETTINGS_PAGE_HH
#define SETTINGS_PAGE_HH

#include "menu_page.hh"

namespace utils {
namespace ui {

/**
 * @class SettingsPage
 * @brief Page de paramètres du système
 *
 * Permet de naviguer entre différents paramètres :
 * - PARAMETER_SENSOR : Configuration des capteurs
 * - PARAMETER_ACTUATOR : Configuration des actionneurs (PWM)
 * - PARAMETER_BACKLIGHT_BRIGHTNESS : Luminosité du rétroéclairage
 */
class SettingsPage : public NavigablePage {

  public:
    void display() override;
    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;

    /**
     * @brief Récupère le paramètre actuellement sélectionné
     * @return Le paramètre sélectionné
     */
    inline display_parameter_t getSelectedParameter() const {
        return selParameter_;
    }

  private:
    display_parameter_t selParameter_ = display_parameter_t::PARAMETER_SENSOR;
};

} // namespace ui
} // namespace utils

#endif // SETTINGS_PAGE_HH
