#ifndef MENU_CONTROLER_HH
#define MENU_CONTROLER_HH

#include "main_page.hh"
#include "menu_page.hh"
#include "pwm_edit_page.hh"
#include "sensors_parameter.hh"
#include "settings_page.hh"

namespace utils {
namespace ui {

enum class EditMode : uint8_t { NAVIGATING, EDITING };

enum class ControllerState : uint8_t { WELCOME_SCREEN, NORMAL_MODE };

class UIControler {
  public:
    static UIControler &getInstance();

    void init(bool showWelcomeScreen = true, uint32_t switchPeriodMs = 5000);
    void update();

    inline void setDelta_m(uint8_t delta) {
        this->delta_m = delta;
    }

  private:
    UIControler();
    UIControler(const UIControler &) = delete;
    UIControler &operator=(const UIControler &) = delete;

    MenuPage *currentPage_;
    MainPage mainPage_;
    SettingsPage settingsPage_;
    PWMEditPage pwmEditPage_;
    SensorTypeSelectPage sensorTypeSelectPage_;
    SensorNumberSelectPage sensorNumberSelectPage_;
    TempOffsetEditPage tempOffsetEditPage_;
    HumidityCalibEditPage humidityCalibEditPage_;

    MenuState currentState_;
    EditMode editMode_;
    ControllerState controllerState_;

    uint32_t lastSwitchTick_;
    uint32_t switchPeriodMs_;
    uint32_t welcomeDurationMs_;
    uint8_t delta_m;

    void pollButtons_();
    void handleButtonUp_();
    void handleButtonDown_();
    void handleButtonSelect_();
    void handleLongPressSelect_();

    void switchToPage_(MenuState newState);
    void displayCurrentPage_();
    void showWelcomeScreen();

    EditPage *getCurrentEditPage_() const;
};

} // namespace ui
} // namespace utils

#endif // MENU_CONTROLER_HH