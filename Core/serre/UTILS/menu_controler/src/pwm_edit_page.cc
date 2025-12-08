#include "../inc/pwm_edit_page.hh"
#include "../../../driver/LCD/inc/lcd.hh"
#include "../../../driver/PWM/inc/pwm_manager.hh"
#include "../inc/menu_controler.hh"

namespace utils {
namespace ui {

void PWMEditPage::display() {
    auto &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();

    auto &pwmManager = driver::pwm::PWMManager::getInstance();
    uint8_t fanDuty = pwmManager.getPWM(pwm_channel_t::FAN)->getDutyCycle();
    uint8_t pumpDuty = pwmManager.getPWM(pwm_channel_t::PUMP)->getDutyCycle();

    // Affichage des deux canaux
    lcd.lcd_write_str("Fan: %u %%\nPump: %u %%", fanDuty, pumpDuty);

    // Curseur sur le channel sélectionné
    uint8_t cursorRow = (selectedChannel_ == pwm_channel_t::FAN) ? 0 : 1;
    lcd.lcd_goto(cursorRow, 15);
    lcd.lcd_write_char('<');
}

void PWMEditPage::onEnterEditMode() {
    UIControler::getInstance().setDelta_m(10);
    this->display();
}

void PWMEditPage::onExitEditMode() {
    // Optionnel : rafraîchir l'affichage
    this->display();
}

void PWMEditPage::adjustValue(int8_t delta) {
    auto &pwmManager = driver::pwm::PWMManager::getInstance();
    auto *pwmChannel = pwmManager.getPWM(selectedChannel_);

    uint8_t currentDuty = pwmChannel->getDutyCycle();
    int16_t newDuty = static_cast<int16_t>(currentDuty) + delta;

    // Clamper entre 0 et 100
    newDuty = (newDuty < 0) ? 0 : (newDuty > 100) ? 100 : newDuty;

    pwmChannel->setDutyCycle(static_cast<uint8_t>(newDuty));
    this->display();
}

void PWMEditPage::navigateSelection(int8_t direction) {
    if (direction != 0) {
        // Basculer entre FAN et PUMP (direction positive ou négative)
        selectedChannel_ = (selectedChannel_ == pwm_channel_t::FAN)
                               ? pwm_channel_t::PUMP
                               : pwm_channel_t::FAN;
    }
    this->display();
}

} // namespace ui
} // namespace utils
