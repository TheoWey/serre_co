#include "main_serre.h"

#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/sensors/inc/sensor_manager.hh"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

void main_serre(void) {
    static adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    if (adcManager.getConversionCompleteFlag()) {
        auto &sensorManager = sensor::SensorManager::getInstance();
        sensorManager.updateAllSensors();
        adcManager.setConversionCompleteFlag(false);
    }
}

void main_serre_init(void) {
    // 1. Initialiser ADC
    adc_manager::ADCManager::initialize(&hadc1, &hdma_adc1);
    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();

    // 3. Créer les capteurs
    static sensor::TempSensor tempSensor0(&adcManager, 1);
    static sensor::SoilHumSensor soilHumSensor0(&adcManager, 0);

    // 4. Enregistrer dans le manager
    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();
    sensorManager.subscribeTempSensor(&tempSensor0);
    sensorManager.subscribeSoilHumSensor(&soilHumSensor0);

    // 5. Démarrer l'ADC
    if (HAL_OK != adcManager.start()) {
        Error_Handler();
    }
}
