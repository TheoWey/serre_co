#include "main_serre.h"

#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/sensors/inc/sensor_manager.hh"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

void main_serre(void) {
    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();

    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    if (adcManager.getConversionCompleteFlag()) {
        auto &sensorManager = sensor::SensorManager::getInstance();
        sensorManager.updateAllSensors();
        adcManager.setConversionCompleteFlag(false);
    }
}

void main_serre_init(void) {
    adc_manager::ADCManager::initialize(&hadc1, &hdma_adc1, &htim2);

    static adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    static sensor::TempSensor tempSensor0(&adcManager, 1);       // Channel 1
    static sensor::SoilHumSensor soilHumSensor0(&adcManager, 0); // Channel 0

    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();
    sensorManager.subscribeTempSensor(&tempSensor0);
    sensorManager.subscribeSoilHumSensor(&soilHumSensor0);

    if (HAL_OK != adcManager.start()) {
        Error_Handler();
    }
}
