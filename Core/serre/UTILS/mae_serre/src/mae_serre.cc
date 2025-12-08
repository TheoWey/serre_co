#include "../inc/mae_serre.hh"

#include "stm32g0xx_hal.h"

#include "../../../driver/PWM/inc/pwm_manager.hh"
#include "../../../driver/sensors/inc/sensor_manager.hh"
#include "../../menu_controler/inc/menu_controler.hh"

namespace utils {
namespace mae_serre {
SerreController &SerreController::getInstance() {
    static SerreController instance;
    return instance;
}

void SerreController::processLoop() {
    uint32_t currentTick = HAL_GetTick();
    switch (this->currentMode_) {
    case SerreMode::NORMAL:
        if (currentTick - this->lastSensorUpdateTick_ >=
            this->sensorUpdatePeriodMs_) {
            this->lastSensorUpdateTick_ = currentTick;
            sensor_update_loop();
        }
        if (currentTick - this->lastPwmUpdateTick_ >=
            this->pwmUpdatePeriodMs_) {
            this->lastPwmUpdateTick_ = currentTick;
            pwm_control_loop();
        }
        break;
    case SerreMode::SETTING:
        driver::pwm::PWMManager::getInstance().enableAll(true);
        this->lastSettingModeTick_ =
            ui::UIControler::getInstance().getLastActionTick();
        if (currentTick - this->lastSettingModeTick_ >=
            this->settingModeTimeoutMs_) {
            this->currentMode_ = SerreMode::NORMAL;
            ui::UIControler::getInstance().setCurrentMenu(ui::MenuState::MAIN);
        }

        break;
    default:
        // Autres modes
        break;
    }
}

void pwm_control_loop(void) {
    using namespace driver::pwm;
    if (driver::sensor::SensorManager::getInstance().getTempSensor(
            temp_channel_t::TEMPERATURE_0) != nullptr) {
        float temperature = driver::sensor::SensorManager::getInstance()
                                .getTempSensor(temp_channel_t::TEMPERATURE_0)
                                ->getTemperatureCelsius();
        if (temperature <
            20.0f) { //------ set parameter to select temperature of
                     // activation/deactivation with hysteresis
            PWMManager::getInstance().enable(pwm_channel_t::FAN, false);
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::FAN,
                                                   0); // Turn off fan
        } else if (temperature > 20.0f) {              //------ set parameter to
            // select temperature of
            // activation/deactivation with hysteresis
            PWMManager::getInstance().enable(pwm_channel_t::FAN, true);
            float dutyCycle = ((temperature * 100.0f) / 50.0f); // Scale 20-40C
            PWMManager::getInstance().setDutyCycle(
                pwm_channel_t::FAN,
                dutyCycle); // Control first PWM channel
        }
    }
    if (driver::sensor::SensorManager::getInstance().getSoilHumSensor(
            hum_channel_t::SOIL_HUMIDITY_0) != nullptr) {
        float soilHumidity =
            driver::sensor::SensorManager::getInstance()
                .getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
                ->getHumidityPercent();
        if (soilHumidity < 20.0f) { //------ set parameter to
                                    // select humidity of
                                    // activation/deactivation with hysteresis
            PWMManager::getInstance().enable(pwm_channel_t::PUMP, false);
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::PUMP,
                                                   0); // Turn off pump
        } else if (soilHumidity >
                   40.0f) { //------ set parameter to
                            // select humidity of
                            // activation/deactivation with hysteresis
            PWMManager::getInstance().enable(pwm_channel_t::PUMP, true);
            float dutyCycle = (100.0f - soilHumidity); // Scale0-100%
            PWMManager::getInstance().setDutyCycle(
                pwm_channel_t::PUMP,
                dutyCycle); // Control second PWMchannel
        }
    }
}

void sensor_update_loop(void) {
    using namespace driver::sensor;
    SensorManager::getInstance().updateAllSensors();
}

} // namespace mae_serre
} // namespace utils
