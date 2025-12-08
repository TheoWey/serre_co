#ifndef MAIN_PAGE_HH
#define MAIN_PAGE_HH

#include "menu_page.hh"

namespace utils {
namespace ui {

class MainPage : public MenuPage {
  public:
    void display() override;

    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;
    void onLongPressUp() override;
    void onLongPressDown() override;
    void onLongPressSelect() override;

    MenuState getNextState() const override {
        return nextState_;
    }

    inline void switchPhase() {
        this->displayPhase_ =
            (this->displayPhase_ == display_phase_t::DISPLAY_TEMP_HUM)
                ? display_phase_t::DISPLAY_PWM
                : display_phase_t::DISPLAY_TEMP_HUM;
    }

  private:
    display_phase_t displayPhase_ = display_phase_t::DISPLAY_TEMP_HUM;
    MenuState nextState_ = MenuState::MAIN;
};

} // namespace ui
} // namespace utils

#endif // MAIN_PAGE_HH
