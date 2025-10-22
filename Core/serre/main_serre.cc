#include "main_serre.h"

#include "../Inc/adc.h"
#include "driver/sensors/soil_hum_sensor/inc/soil_hum.hh"
#include "driver/sensors/temp_sensor/inc/temp_sensor.hh"

#include "driver/PWM/inc/pwm.hh"

void main_serre(void) {

    static sensor::SensorConfig tempConfig = {&hadc1, ADC_CHANNEL_1,
                                              ADC_SAMPLINGTIME_COMMON_1, 100};

    static sensor::SensorConfig soilHumConfig = {
        &hadc1, ADC_CHANNEL_0, ADC_SAMPLINGTIME_COMMON_1, 100};

    static pwm::pwm_handler_t pwm_pumpHandler = {&htim1, TIM_CHANNEL_1};
    static pwm::pwm_handler_t pwm_fanHandler = {&htim1, TIM_CHANNEL_2};

    static sensor::SoilHumSensor soilHumSensor(soilHumConfig);
    static sensor::TempSensor tempSensor(tempConfig);

    static pwm::PWM pumpPWM(pwm_pumpHandler);
    static pwm::PWM fanPWM(pwm_fanHandler);

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

    pumpPWM.setDutyCycle(humidity / 100.0f);
    fanPWM.setDutyCycle(temperature / 100.0f);

    if (temperature < 20.0f) {
        fanPWM.disable();
    } else {
        fanPWM.enable();
    }

    if (humidity < 40.0f) {
        pumpPWM.enable();
    } else {
        pumpPWM.disable();
    }

    HAL_Delay(1000);
}
