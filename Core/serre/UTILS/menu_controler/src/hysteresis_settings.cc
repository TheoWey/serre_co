#include "../inc/hysteresis_settings.hh"
#include "../../../driver/LCD/inc/lcd.hh"
#include "../../mae_serre/inc/mae_serre.hh"
#include "../inc/menu_controler.hh"

namespace utils {
namespace ui {

void SetpointTypeSelectPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Temperature");
    lcd.lcd_goto(1, 0);
    lcd.lcd_write_str("Humidity");
    lcd.lcd_goto(this->selectedType_ == SetpointType::TEMPERATURE ? 0 : 1, 15);
    lcd.lcd_write_char('<');
}

void SetpointTypeSelectPage::onButtonUp() {
    this->selectedType_ = (this->selectedType_ == SetpointType::TEMPERATURE)
                              ? SetpointType::HUMIDITY
                              : SetpointType::TEMPERATURE;
    this->display();
}

void SetpointTypeSelectPage::onButtonDown() {
    this->onButtonUp();
}

void SetpointTypeSelectPage::onButtonSelect() {
}

void SetpointModeSelectPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();
    const char *typeStr =
        (selectedType_ == SetpointType::TEMPERATURE) ? "Temp" : "Hum";
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("%s Setpoint", typeStr);
    lcd.lcd_goto(1, 0);
    lcd.lcd_write_str("%s Reset", typeStr);
    lcd.lcd_goto(this->selectedMode_ == SetpointMode::SETPOINT ? 0 : 1, 15);
    lcd.lcd_write_char('<');
}

void SetpointModeSelectPage::onButtonUp() {
    this->selectedMode_ = (this->selectedMode_ == SetpointMode::SETPOINT)
                              ? SetpointMode::RESET_POINT
                              : SetpointMode::SETPOINT;
    this->display();
}

void SetpointModeSelectPage::onButtonDown() {
    this->onButtonUp();
}

void SetpointModeSelectPage::onButtonSelect() {
}

void SetpointValueEditPage::setSensor(SetpointType type, SetpointMode mode) {
    this->selectedType_ = type;
    this->selectedMode_ = mode;
    // Charger les valeurs actuelles depuis le contrôleur
    auto &controller = mae_serre::SerreController::getInstance();
    if (type == SetpointType::TEMPERATURE) {
        this->temperatureSetpoint_ = controller.GetTemperatureSetpoint();
        this->temperatureResetPoint_ = controller.GetTemperatureResetPoint();
    } else {
        this->humiditySetpoint_ = controller.GetHumiditySetpoint();
        this->humidityResetPoint_ = controller.GetHumidityResetPoint();
    }
}

void SetpointValueEditPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();

    const char *typeStr =
        (this->selectedType_ == SetpointType::TEMPERATURE) ? "Temp" : "Hum";
    const char *modeStr =
        (this->selectedMode_ == SetpointMode::SETPOINT) ? "Setpoint" : "Reset";

    uint16_t value;
    if (this->selectedType_ == SetpointType::TEMPERATURE) {
        value = (this->selectedMode_ == SetpointMode::SETPOINT)
                    ? this->temperatureSetpoint_
                    : this->temperatureResetPoint_;
        lcd.lcd_goto(0, 0);
        lcd.lcd_write_str("%s %s", typeStr, modeStr);
        lcd.lcd_goto(1, 0);
        lcd.lcd_write_str("Value: %u C", value);
    } else {
        value = (this->selectedMode_ == SetpointMode::SETPOINT)
                    ? this->humiditySetpoint_
                    : this->humidityResetPoint_;
        lcd.lcd_goto(0, 0);
        lcd.lcd_write_str("%s %s", typeStr, modeStr);
        lcd.lcd_goto(1, 0);
        lcd.lcd_write_str("Value: %u %%", value);
    }
}

void SetpointValueEditPage::onEnterEditMode() {
    UIControler::getInstance().setDelta_m(10);
    if (this->selectedType_ == SetpointType::TEMPERATURE) {
        this->temperatureSetpoint_ =
            mae_serre::SerreController::getInstance().GetTemperatureSetpoint();
        this->temperatureResetPoint_ = mae_serre::SerreController::getInstance()
                                           .GetTemperatureResetPoint();
    } else {
        this->humiditySetpoint_ =
            mae_serre::SerreController::getInstance().GetHumiditySetpoint();
        this->humidityResetPoint_ =
            mae_serre::SerreController::getInstance().GetHumidityResetPoint();
    }
    this->display();
}

void SetpointValueEditPage::onExitEditMode() {
    // Sauvegarder les valeurs dans le contrôleur
    auto &controller = mae_serre::SerreController::getInstance();
    if (this->selectedType_ == SetpointType::TEMPERATURE) {
        controller.SetTemperatureSetpoint(this->temperatureSetpoint_);
        controller.SetTemperatureResetPoint(this->temperatureResetPoint_);
    } else {
        controller.SetHumiditySetpoint(this->humiditySetpoint_);
        controller.SetHumidityResetPoint(this->humidityResetPoint_);
    }
    this->display();
}

void SetpointValueEditPage::adjustValue(int8_t delta) {
    if (this->selectedType_ == SetpointType::TEMPERATURE) {
        if (this->selectedMode_ == SetpointMode::SETPOINT) {
            int32_t val =
                static_cast<int32_t>(this->temperatureSetpoint_) + delta;
            this->temperatureSetpoint_ =
                static_cast<uint16_t>(val < 0 ? 0 : (val > 100 ? 100 : val));
        } else {
            int32_t val =
                static_cast<int32_t>(this->temperatureResetPoint_) + delta;
            this->temperatureResetPoint_ =
                static_cast<uint16_t>(val < 0 ? 0 : (val > 100 ? 100 : val));
        }
    } else {
        if (this->selectedMode_ == SetpointMode::SETPOINT) {
            int32_t val = static_cast<int32_t>(this->humiditySetpoint_) + delta;
            this->humiditySetpoint_ =
                static_cast<uint16_t>(val < 0 ? 0 : (val > 100 ? 100 : val));
        } else {
            int32_t val =
                static_cast<int32_t>(this->humidityResetPoint_) + delta;
            this->humidityResetPoint_ =
                static_cast<uint16_t>(val < 0 ? 0 : (val > 100 ? 100 : val));
        }
    }
    this->display();
}

} // namespace ui
} // namespace utils
