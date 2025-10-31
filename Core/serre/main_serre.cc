#include "main_serre.h"

#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/sensors/inc/sensor_manager.hh"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

void main_serre(void) {
    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();

    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance(nullptr, nullptr);

    if (adcManager.getConversionCompleteFlag()) {
        sensorManager.readAllSensors();
        sensorManager.processAllSensors();
        adcManager.setConversionCompleteFlag(false);
    }
}

void config(void) {
    static adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance(&hadc1, &hdma_adc1);

    static sensor::TempSensor tempSensor0(&adcManager, 1);
    static sensor::SoilHumSensor soilHumSensor0(&adcManager, 0);

    static sensor::SensorManager &sensorManager =
        sensor::SensorManager::getInstance();

    sensorManager.subscribeTempSensor(&tempSensor0);
    sensorManager.subscribeSoilHumSensor(&soilHumSensor0);

    if (HAL_OK != adcManager.start()) {
        Error_Handler();
    }
}
