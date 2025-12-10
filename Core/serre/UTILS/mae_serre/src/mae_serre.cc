#include "../inc/mae_serre.hh"

#include "stm32g0xx_hal.h"

#include "../../../driver/PWM/inc/pwm_manager.hh"
#include "../../../driver/sensors/inc/sensor_manager.hh"
#include "../../menu_controler/inc/menu_controler.hh"

using namespace utils::mae_serre;

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
            this->sensor_update_loop();
        }
        if (currentTick - this->lastPwmUpdateTick_ >=
            this->pwmUpdatePeriodMs_) {
            this->lastPwmUpdateTick_ = currentTick;
            this->pwm_control_loop();
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
        // not implemented case
        break;
    }
}

void SerreController::pwm_control_loop(void) {
    using namespace driver::pwm;
    if (driver::sensor::SensorManager::getInstance().getTempSensor(
            temp_channel_t::TEMPERATURE_0) != nullptr) {
        float temperature = driver::sensor::SensorManager::getInstance()
                                .getTempSensor(temp_channel_t::TEMPERATURE_0)
                                ->getTemperatureCelsius();
        if (temperature < this->temperatureResetPoint_) {
            PWMManager::getInstance().enable(pwm_channel_t::FAN, false);
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::FAN, 0);
        } else if (temperature > this->temperatureSetpoint_) {
            PWMManager::getInstance().enable(pwm_channel_t::FAN, true);
            float dutyCycle = ((temperature - this->temperatureResetPoint_) /
                               (60.0f - this->temperatureResetPoint_)) *
                              100.0f;
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::FAN,
                                                   dutyCycle);
        }
    }
    if (driver::sensor::SensorManager::getInstance().getSoilHumSensor(
            hum_channel_t::SOIL_HUMIDITY_0) != nullptr) {
        float soilHumidity =
            driver::sensor::SensorManager::getInstance()
                .getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
                ->getHumidityPercent();
        if (soilHumidity > this->humidityResetPoint_) {
            PWMManager::getInstance().enable(pwm_channel_t::PUMP, false);
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::PUMP, 0);
        } else if (soilHumidity < this->humiditySetpoint_) {
            PWMManager::getInstance().enable(pwm_channel_t::PUMP, true);
            float dutyCycle = ((this->humiditySetpoint_ - soilHumidity) /
                               this->humiditySetpoint_) *
                              100.0f;
            PWMManager::getInstance().setDutyCycle(pwm_channel_t::PUMP,
                                                   dutyCycle);
        }
    }
}

void SerreController::sensor_update_loop(void) {
    using namespace driver::sensor;
    SensorManager::getInstance().updateAllSensors();
}
