#include "main_serre.h"

#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/PWM/inc/pwm_manager.hh"
#include "driver/sensors/inc/sensor_manager.hh"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

void main_serre(void) {

    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    if (adcManager.getConversionCompleteFlag()) {
        pwm::PWMManager::getInstance().enableAll(false);
        auto &sensorManager = sensor::SensorManager::getInstance();
        sensorManager.updateAllSensors();
        adcManager.setConversionCompleteFlag(false);

        pwm::PWMManager &pwmManager = pwm::PWMManager::getInstance();

        if (sensorManager.getTempSensor(0) != nullptr) {
            float temperature =
                sensorManager.getTempSensor(0)->getTemperatureCelsius();
            // Simple control logic: increase PWM duty cycle with temperature
            float dutyCycle =
                ((temperature * 100.0f) / 50.0f) / 100; // Scale 20-40C
            if (dutyCycle < 0.0f)
                dutyCycle = 0.0f;
            if (dutyCycle > 1.0f)
                dutyCycle = 1.0f;
            pwmManager.setDutyCycle(0, dutyCycle); // Control first PWM channel
        }
        if (sensorManager.getSoilHumSensor(0) != nullptr) {
            float soilHumidity =
                sensorManager.getSoilHumSensor(0)->getHumidityPercent();
            // Simple control logic: decrease PWM duty cycle with humidity
            float dutyCycle = (100.0f - soilHumidity) / 100.0f; // Scale 0-100%
            if (dutyCycle < 0.0f)
                dutyCycle = 0.0f;
            if (dutyCycle > 1.0f)
                dutyCycle = 1.0f;
            pwmManager.setDutyCycle(1, dutyCycle); // Control second PWM channel
        }
    }
    pwm::PWMManager::getInstance().enableAll(true);
}

void main_serre_init(void) {
    adc_manager::ADCManager::initialize(&hadc1, &hdma_adc1, &htim2);

    static adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    static sensor::TempSensor tempSensor0(&adcManager, 1);       // Channel 1
    static sensor::SoilHumSensor soilHumSensor0(&adcManager, 0); // Channel 0

    pwm::pwm_handler_t pwm_handlers[] = {{&htim1, TIM_CHANNEL_1},
                                         {&htim1, TIM_CHANNEL_2}

    };
    pwm::PWMManager::initialize(pwm_handlers, 2); // No PWM channels for now

    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();
    sensorManager.subscribeTempSensor(&tempSensor0);
    sensorManager.subscribeSoilHumSensor(&soilHumSensor0);

    if (HAL_OK != adcManager.start()) {
        Error_Handler();
    }
}
