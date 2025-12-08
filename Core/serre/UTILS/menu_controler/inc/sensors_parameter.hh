#ifndef SENSORS_PARAMETER_HH
#define SENSORS_PARAMETER_HH

#include "menu_page.hh"

namespace utils {
namespace ui {

enum class SensorType { TEMPERATURE, HUMIDITY };

class SensorTypeSelectPage : public NavigablePage {
  public:
    SensorTypeSelectPage() = default;
    void display() override;
    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;
    SensorType getSelectedType() const {
        return selectedType_;
    }
    MenuState getNextState() const override {
        return MenuState::SENSOR_NUMBER_SELECT;
    }

  private:
    SensorType selectedType_ = SensorType::TEMPERATURE;
};

class SensorNumberSelectPage : public NavigablePage {
  public:
    SensorNumberSelectPage() = default;
    void setType(SensorType type) {
        selectedType_ = type;
        selectedIndex_ = 0;
    }
    void display() override;
    void onButtonUp() override;
    void onButtonDown() override;
    void onButtonSelect() override;
    SensorType getSelectedType() const {
        return selectedType_;
    }
    uint8_t getSelectedIndex() const {
        return selectedIndex_;
    }
    MenuState getNextState() const override {
        return (selectedType_ == SensorType::TEMPERATURE)
                   ? MenuState::TEMP_OFFSET_EDIT
                   : MenuState::HUMIDITY_CALIB_EDIT;
    }

  private:
    SensorType selectedType_ = SensorType::TEMPERATURE;
    uint8_t selectedIndex_ = 0;
};

class TempOffsetEditPage : public EditPage {
  public:
    TempOffsetEditPage() = default;
    void setSensor(uint8_t index) {
        selectedSensor_ = index;
    }
    void display() override;
    void onEnterEditMode() override;
    void onExitEditMode() override;
    void adjustValue(int8_t delta) override;
    void navigateSelection(int8_t direction) override;

  private:
    uint8_t selectedSensor_ = 0;
    uint16_t tempOffset_[5] = {20, 20, 20, 20, 20};
};

class HumidityCalibEditPage : public EditPage {
  public:
    HumidityCalibEditPage() = default;
    void setSensor(uint8_t index) {
        selectedSensor_ = index;
        editField_ = 0;
    }
    void display() override;
    void onEnterEditMode() override;
    void onExitEditMode() override;
    void adjustValue(int8_t delta) override;
    void navigateSelection(int8_t direction) override;

  private:
    uint8_t selectedSensor_ = 0;
    uint8_t editField_ = 0;
    uint16_t dry_[5] = {4095, 4095, 4095, 4095, 4095};
    uint16_t wet_[5] = {0, 0, 0, 0, 0};
};

} // namespace ui
} // namespace utils

#endif // SENSORS_PARAMETER_HH
