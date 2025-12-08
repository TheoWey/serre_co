#include "../inc/settings_page.hh"
#include "../../../driver/LCD/inc/lcd.hh"

namespace utils {
namespace ui {

void SettingsPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();

    if (this->selParameter_ == display_parameter_t::PARAMETER_SENSOR ||
        this->selParameter_ == display_parameter_t::PARAMETER_ACTUATOR) {
        lcd.lcd_write_str("Sens. Settings\nActu. Settings");
    } else {
        lcd.lcd_write_str("Setpoint Settings\n");
    }

    uint8_t cursorRow = static_cast<uint8_t>(this->selParameter_) % 2;
    lcd.lcd_goto(cursorRow, 15);
    lcd.lcd_write_char('<');
}

void SettingsPage::onButtonUp() {
    this->selParameter_ =
        (this->selParameter_ == display_parameter_t::PARAMETER_SENSOR)
            ? display_parameter_t::PARAMETER_SETPOINT
            : static_cast<display_parameter_t>(
                  static_cast<uint8_t>(this->selParameter_) - 1);
}

void SettingsPage::onButtonDown() {
    this->selParameter_ =
        (this->selParameter_ == display_parameter_t::PARAMETER_SETPOINT)
            ? display_parameter_t::PARAMETER_SENSOR
            : static_cast<display_parameter_t>(
                  static_cast<uint8_t>(this->selParameter_) + 1);
}

void SettingsPage::onButtonSelect() {
    // Ne rien faire ici - la transition est gérée par MenuControler
}

} // namespace ui
} // namespace utils
