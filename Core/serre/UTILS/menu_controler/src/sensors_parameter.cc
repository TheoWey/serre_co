#include "../inc/sensors_parameter.hh"
#include "../../../driver/LCD/inc/lcd.hh"
#include "../../../driver/sensors/inc/sensor_manager.hh"
#include "../inc/menu_controler.hh"

namespace utils {
namespace ui {

void SensorTypeSelectPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("Temperature");
    lcd.lcd_goto(1, 0);
    lcd.lcd_write_str("Humidity");
    lcd.lcd_goto(selectedType_ == SensorType::TEMPERATURE ? 0 : 1, 15);
    lcd.lcd_write_char('<');
}
void SensorTypeSelectPage::onButtonUp() {
    selectedType_ = (selectedType_ == SensorType::TEMPERATURE)
                        ? SensorType::HUMIDITY
                        : SensorType::TEMPERATURE;
    display();
}
void SensorTypeSelectPage::onButtonDown() {
    onButtonUp();
}
void SensorTypeSelectPage::onButtonSelect() {
}

void SensorNumberSelectPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();

    const char *prefix =
        (selectedType_ == SensorType::TEMPERATURE) ? "Temp" : "Hum";

    uint8_t firstSensor = (this->selectedIndex_ >= 3)   ? 3
                          : (this->selectedIndex_ >= 1) ? 1
                                                        : 0;
    uint8_t firstLine = this->selectedIndex_ - firstSensor;

    lcd.lcd_goto(0, 0);
    lcd.lcd_write_str("%s #%d", prefix, firstSensor + 1);

    if (firstSensor < 4) {
        lcd.lcd_goto(1, 0);
        lcd.lcd_write_str("%s #%d", prefix, firstSensor + 2);
    }

    lcd.lcd_goto(firstLine, 15);
    lcd.lcd_write_char('<');
}

void SensorNumberSelectPage::onButtonUp() {
    selectedIndex_ = (selectedIndex_ == 0) ? 4 : selectedIndex_ - 1;
    display();
}

void SensorNumberSelectPage::onButtonDown() {
    selectedIndex_ = (selectedIndex_ == 4) ? 0 : selectedIndex_ + 1;
    display();
}

void SensorNumberSelectPage::onButtonSelect() {
}

void TempOffsetEditPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_write_str("Temp #%u\nOffset: %d", selectedSensor_ + 1,
                      tempOffset_[selectedSensor_]);
}
void TempOffsetEditPage::onEnterEditMode() {
    UIControler::getInstance().setDelta_m(1);
    display();
}
void TempOffsetEditPage::onExitEditMode() {
    driver::sensor::SensorManager::getInstance()
        .getTempSensor(static_cast<temp_channel_t>(selectedSensor_))
        ->setOffset(tempOffset_[selectedSensor_]);
    display();
}
void TempOffsetEditPage::adjustValue(int8_t delta) {
    tempOffset_[selectedSensor_] += delta;
    display();
}

void TempOffsetEditPage::navigateSelection(int8_t) {
}

void HumidityCalibEditPage::display() {
    driver::lcd::LCD &lcd = driver::lcd::LCD::getInstance();
    lcd.lcd_clear();
    lcd.lcd_write_str("Dry: %u\nWet: %u", dry_[selectedSensor_],
                      wet_[selectedSensor_]);
    lcd.lcd_goto(editField_, 15);
    lcd.lcd_write_char('<');
}
void HumidityCalibEditPage::onEnterEditMode() {
    UIControler::getInstance().setDelta_m(10);
    driver::sensor::SensorManager::getInstance()
        .getSoilHumSensor(static_cast<hum_channel_t>(selectedSensor_))
        ->getCalibration(dry_[selectedSensor_], wet_[selectedSensor_]);
    display();
}
void HumidityCalibEditPage::onExitEditMode() {
    driver::sensor::SensorManager::getInstance()
        .getSoilHumSensor(static_cast<hum_channel_t>(selectedSensor_))
        ->calibrate(dry_[selectedSensor_], wet_[selectedSensor_]);
    editField_ = 0;
    display();
}
void HumidityCalibEditPage::adjustValue(int8_t delta) {
    if (editField_ == 0) {
        int32_t val = static_cast<int32_t>(dry_[selectedSensor_]) + delta;
        dry_[selectedSensor_] =
            static_cast<uint16_t>(val < 0 ? 0 : (val > 4095 ? 4095 : val));
    } else {
        int32_t val = static_cast<int32_t>(wet_[selectedSensor_]) + delta;
        wet_[selectedSensor_] =
            static_cast<uint16_t>(val < 0 ? 0 : (val > 4095 ? 4095 : val));
    }
    display();
}
void HumidityCalibEditPage::navigateSelection(int8_t direction) {
    editField_ = (editField_ == 0) ? 1 : 0;
    display();
}

} // namespace ui
} // namespace utils
