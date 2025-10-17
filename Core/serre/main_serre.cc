#include "main_serre.h"

#include "../Inc/adc.h"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

void main_serre(void) {

    static sensor::SensorConfig tempConfig = {&hadc1, ADC_CHANNEL_1,
                                              ADC_SAMPLINGTIME_COMMON_1, 100};

    static sensor::SensorConfig soilHumConfig = {
        &hadc1, ADC_CHANNEL_0, ADC_SAMPLINGTIME_COMMON_1, 100};

    static sensor::SoilHumSensor soilHumSensor(soilHumConfig);
    static sensor::TempSensor tempSensor(tempConfig);

    float temperature = 0.0f;
    float humidity = 0.0f;

    if (tempSensor.readData() == HAL_OK) {
        tempSensor.processData();
        temperature = tempSensor.getTemperatureCelsius();
    }

    if (soilHumSensor.readData() == HAL_OK) {
        soilHumSensor.processData();
        humidity = soilHumSensor.getHumidityPercent();
    }
    HAL_Delay(1000);
}
