#ifndef HYSTERESIS_SETTINGS_HH
#define HYSTERESIS_SETTINGS_HH

#include "menu_page.hh"

namespace utils {
namespace ui {

enum class SetpointType { TEMPERATURE, HUMIDITY };
enum class SetpointMode { SETPOINT, RESET_POINT };

class SetpointTypeSelectPage : public NavigablePage {
  public:
    SetpointTypeSelectPage() = default;
    void display() override;
    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;
    SetpointType getSelectedType() const {
        return selectedType_;
    }
    MenuState getNextState() const override {
        return MenuState::SETPOINT_MODE_SELECT;
    }

  private:
    SetpointType selectedType_ = SetpointType::TEMPERATURE;
};

class SetpointModeSelectPage : public NavigablePage {
  public:
    SetpointModeSelectPage() = default;
    void setType(SetpointType type) {
        selectedType_ = type;
        selectedMode_ = SetpointMode::SETPOINT;
    }
    void display() override;
    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;
    SetpointType getSelectedType() const {
        return selectedType_;
    }
    SetpointMode getSelectedMode() const {
        return selectedMode_;
    }
    MenuState getNextState() const override {
        return MenuState::SETPOINT_VALUE_EDIT;
    }

  private:
    SetpointType selectedType_ = SetpointType::TEMPERATURE;
    SetpointMode selectedMode_ = SetpointMode::SETPOINT;
};

class SetpointValueEditPage : public EditPage {
  public:
    SetpointValueEditPage() = default;
    void setSensor(SetpointType type, SetpointMode mode);
    void display() override;
    void onEnterEditMode() override;
    void onExitEditMode() override;
    void adjustValue(int8_t delta) override;
    void navigateSelection(int8_t direction) override {
    }

  private:
    SetpointType selectedType_ = SetpointType::TEMPERATURE;
    SetpointMode selectedMode_ = SetpointMode::SETPOINT;
    uint16_t temperatureSetpoint_ = 25;
    uint16_t temperatureResetPoint_ = 30;
    uint16_t humiditySetpoint_ = 50;
    uint16_t humidityResetPoint_ = 60;
};

} // namespace ui
} // namespace utils

#endif // SETPOINT_SETTINGS_HH
