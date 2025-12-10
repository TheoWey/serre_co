#ifndef SENSORS_PARAMETER_HH
#define SENSORS_PARAMETER_HH

/**
 * @file sensors_parameter.hh
 * @brief Menu pages for selecting and editing sensor parameters.
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 * Declares UI pages to choose a sensor type, pick a sensor index, and edit
 * temperature offsets or soil humidity calibration points.
 */

#include "menu_page.hh"

/**
 * @namespace utils
 * @brief Contains utility classes and methods.
 */
namespace utils {
/**
 * @namespace ui
 * @brief Contains classes and methods for user interface management.
 */
namespace ui {

/**
 * @brief Supported sensor categories for configuration.
 */
enum class SensorType { TEMPERATURE, HUMIDITY };

/**
 * @class SensorTypeSelectPage
 * @brief Menu page to pick between temperature and humidity sensors.
 */
class SensorTypeSelectPage : public NavigablePage {
  public:
    SensorTypeSelectPage() = default;

    /**
     * @brief Render the sensor type selection page.
     * @return void
     */
    void display() override;

    /**
     * @brief Move selection up (cycles between types).
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Move selection down (cycles between types).
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Confirm the current sensor type choice.
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Get the currently selected sensor type.
     * @return Selected SensorType
     */
    inline SensorType getSelectedType() const {
        return selectedType_;
    }

    /**
     * @brief Destination state after selecting the type.
     * @return MenuState::SENSOR_NUMBER_SELECT
     */
    inline MenuState getNextState() const override {
        return MenuState::SENSOR_NUMBER_SELECT;
    }

  private:
    SensorType selectedType_ =
        SensorType::TEMPERATURE; ///< Active sensor type selection
};

/**
 * @class SensorNumberSelectPage
 * @brief Menu page to select which sensor index to configure.
 */
class SensorNumberSelectPage : public NavigablePage {
  public:
    SensorNumberSelectPage() = default;

    /**
     * @brief Set the sensor type and reset selection index.
     * @param type SensorType to configure
     * @return void
     */
    void setType(SensorType type) {
        selectedType_ = type;
        selectedIndex_ = 0;
    }

    /**
     * @brief Render the sensor number selection page.
     * @return void
     */
    void display() override;

    /**
     * @brief Move selection up through sensor indices.
     * @return void
     */
    void onButtonUp() override;

    /**
     * @brief Move selection down through sensor indices.
     * @return void
     */
    void onButtonDown() override;

    /**
     * @brief Confirm the chosen sensor index and proceed.
     * @return void
     */
    void onButtonSelect() override;

    /**
     * @brief Get the selected sensor type.
     * @return SensorType currently configured
     */
    inline SensorType getSelectedType() const {
        return selectedType_;
    }

    /**
     * @brief Get the selected sensor index.
     * @return Index in the configured sensor list
     */
    inline uint8_t getSelectedIndex() const {
        return selectedIndex_;
    }

    /**
     * @brief Destination state based on selected type.
     * @return TEMP_OFFSET_EDIT for temperature, HUMIDITY_CALIB_EDIT otherwise
     */
    inline MenuState getNextState() const override {
        return (selectedType_ == SensorType::TEMPERATURE)
                   ? MenuState::TEMP_OFFSET_EDIT
                   : MenuState::HUMIDITY_CALIB_EDIT;
    }

  private:
    SensorType selectedType_ =
        SensorType::TEMPERATURE; ///< Sensor type being configured
    uint8_t selectedIndex_ = 0;  ///< Selected sensor index
};

/**
 * @class TempOffsetEditPage
 * @brief Edit page to tune temperature sensor offsets.
 */
class TempOffsetEditPage : public EditPage {
  public:
    TempOffsetEditPage() = default;

    /**
     * @brief Select which temperature sensor to edit.
     * @param index Sensor index to edit
     * @return void
     */
    inline void setSensor(uint8_t index) {
        selectedSensor_ = index;
    }

    /**
     * @brief Render the temperature offset edit page.
     * @return void
     */
    void display() override;

    /**
     * @brief Enter edit mode for offset adjustment.
     * @return void
     */
    void onEnterEditMode() override;

    /**
     * @brief Exit edit mode after changes.
     * @return void
     */
    void onExitEditMode() override;

    /**
     * @brief Adjust the offset by a signed delta.
     * @param delta Increment (positive/negative)
     * @return void
     */
    void adjustValue(int8_t delta) override;

    /**
     * @brief Navigate selection (if multiple fields exist).
     * @param direction Navigation direction
     * @return void
     */
    void navigateSelection(int8_t direction) override;

  private:
    uint8_t selectedSensor_ = 0; ///< Sensor index being edited
    uint16_t tempOffset_[5] = {
        20, 20, 20, 20, 20}; ///< Offsets per temperature sensor (0.1°C units)
};

/**
 * @class HumidityCalibEditPage
 * @brief Edit page to set soil humidity dry/wet calibration points.
 */
class HumidityCalibEditPage : public EditPage {
  public:
    HumidityCalibEditPage() = default;

    /**
     * @brief Select which humidity sensor to edit and reset field selection.
     * @param index Sensor index to edit
     * @return void
     */
    void setSensor(uint8_t index) {
        selectedSensor_ = index;
        editField_ = 0;
    }

    /**
     * @brief Render the humidity calibration edit page.
     * @return void
     */
    void display() override;

    /**
     * @brief Enter edit mode for calibration values.
     * @return void
     */
    void onEnterEditMode() override;

    /**
     * @brief Exit edit mode after changes.
     * @return void
     */
    void onExitEditMode() override;

    /**
     * @brief Adjust the selected calibration value by delta.
     * @param delta Increment (positive/negative)
     * @return void
     */
    void adjustValue(int8_t delta) override;

    /**
     * @brief Move between calibration fields (dry vs wet).
     * @param direction Navigation direction
     * @return void
     */
    void navigateSelection(int8_t direction) override;

  private:
    uint8_t selectedSensor_ = 0; ///< Sensor index being calibrated
    uint8_t editField_ = 0;      ///< 0=dry point, 1=wet point
    uint16_t dry_[5] = {4095, 4095, 4095, 4095,
                        4095};          ///< ADC counts for dry reference
    uint16_t wet_[5] = {0, 0, 0, 0, 0}; ///< ADC counts for wet reference
};

} // namespace ui
} // namespace utils

#endif // SENSORS_PARAMETER_HH
