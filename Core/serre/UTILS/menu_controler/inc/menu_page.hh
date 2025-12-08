#ifndef MENU_PAGE_HH
#define MENU_PAGE_HH

#include <stdint.h>

enum hum_channel_t {
    SOIL_HUMIDITY_0,
    SOIL_HUMIDITY_1,
    SOIL_HUMIDITY_2,
    SOIL_HUMIDITY_3,
    SOIL_HUMIDITY_4
};

enum temp_channel_t {
    TEMPERATURE_0,
    TEMPERATURE_1,
    TEMPERATURE_2,
    TEMPERATURE_3,
    TEMPERATURE_4
};

enum i2c_channel_t { I2C_CHANNEL_0, I2C_CHANNEL_1 };

enum pwm_channel_t { FAN, PUMP };

enum bebouncer_channel_t { BUTTON_UP, BUTTON_DOWN, BUTTON_SELECT };

enum class HysteresisParameter : uint8_t {
    TEMPERATURE_HYSTERESIS,
    HUMIDITY_HYSTERESIS
};

namespace utils {
namespace ui {

enum class MenuState {
    MAIN,
    SETTINGS,
    PWM_EDIT,
    SENSORS_PARAMETER,
    SENSOR_NUMBER_SELECT,
    TEMP_OFFSET_EDIT,
    HUMIDITY_CALIB_EDIT,
    SETPOINT_TYPE_SELECT,
    SETPOINT_MODE_SELECT,
    SETPOINT_VALUE_EDIT
};

enum class display_phase_t { DISPLAY_TEMP_HUM, DISPLAY_PWM };

enum class display_parameter_t {
    PARAMETER_SENSOR,
    PARAMETER_ACTUATOR,
    PARAMETER_SETPOINT
};

class MenuPage {
  public:
    virtual ~MenuPage() = default;

    virtual void display() = 0;
    virtual void onButtonUp() = 0;
    virtual void onButtonDown() = 0;
    virtual void onButtonSelect() = 0;
    virtual void onLongPressUp() = 0;
    virtual void onLongPressDown() = 0;
    virtual void onLongPressSelect() = 0;

    virtual MenuState getNextState() const {
        return MenuState::MAIN;
    }

  protected:
    display_phase_t displayPhase_ = display_phase_t::DISPLAY_TEMP_HUM;
};

class NavigablePage : public MenuPage {
  public:
    virtual ~NavigablePage() = default;

    void onLongPressUp() override {
    }
    void onLongPressDown() override {
    }
    void onLongPressSelect() override {
    }

    MenuState getNextState() const override {
        return MenuState::MAIN;
    }

  protected:
    NavigablePage() = default;
};

class EditPage : public MenuPage {
  public:
    virtual ~EditPage() = default;

    virtual void onEnterEditMode() = 0;
    virtual void onExitEditMode() = 0;
    virtual void adjustValue(int8_t delta) = 0;
    virtual void navigateSelection(int8_t direction) = 0;

    void onButtonUp() override {
        navigateSelection(-1);
    }

    void onButtonDown() override {
        navigateSelection(1);
    }

    void onButtonSelect() override {
    }
    void onLongPressUp() override {
    }
    void onLongPressDown() override {
    }
    void onLongPressSelect() override {
    }

    MenuState getNextState() const override {
        return MenuState::SETTINGS;
    }

  protected:
    EditPage() = default;
};

} // namespace ui
} // namespace utils

#endif // MENU_PAGE_HH
