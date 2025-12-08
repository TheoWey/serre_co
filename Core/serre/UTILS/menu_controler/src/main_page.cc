#include "../inc/main_page.hh"
#include "../../../driver/LCD/inc/lcd.hh"
#include "../../../driver/PWM/inc/pwm_manager.hh"
#include "../../../driver/sensors/inc/sensor_manager.hh"

namespace utils {
namespace ui {

void MainPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();

    if (this->displayPhase_ == display_phase_t::DISPLAY_TEMP_HUM) {
        float temperature = driver::sensor::SensorManager::getInstance()
                                .getTempSensor(temp_channel_t::TEMPERATURE_0)
                                ->getTemperatureCelsius();
        float humidity = driver::sensor::SensorManager::getInstance()
                             .getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
                             ->getHumidityPercent();
        lcd.lcd_write_str("Temp: %.1f C\nHum: %.1f %%", temperature, humidity);
    } else {
        uint8_t fanDuty = driver::pwm::PWMManager::getInstance()
                              .getPWM(pwm_channel_t::FAN)
                              ->getDutyCycle();
        uint8_t pumpDuty = driver::pwm::PWMManager::getInstance()
                               .getPWM(pwm_channel_t::PUMP)
                               ->getDutyCycle();
        lcd.lcd_write_str("Fan: %u %%\nPump: %u %%", fanDuty, pumpDuty);
    }
}

void MainPage::onButtonSelect() {
}

void MainPage::onLongPressSelect() {
    nextState_ = MenuState::SETTINGS;
}

void MainPage::onButtonUp() {
}

void MainPage::onButtonDown() {
}

void MainPage::onLongPressUp() {
}

void MainPage::onLongPressDown() {
}

} // namespace ui
} // namespace utils
