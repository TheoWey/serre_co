#include "main_serre.h"

#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/I2C/inc/i2c_manager.hh"
#include "driver/LCD/inc/lcd.hh"
#include "driver/LCD/inc/printer.hh"
#include "driver/PWM/inc/pwm_manager.hh"
#include "driver/UTILS/debouncer/inc/debouncer.hh"
#include "driver/sensors/inc/sensor_manager.hh"

void main_serre(void) {

    adc_manager::ADCManager &adcManager =
        adc_manager::ADCManager::getInstance();
    utils::debouncer::DebouncerManager::getInstance().updateAllDebouncers();

    if (adcManager.getConversionCompleteFlag()) {
        pwm::PWMManager::getInstance().enableAll(false);
        auto &sensorManager = sensor::SensorManager::getInstance();
        sensorManager.updateAllSensors();
        adcManager.setConversionCompleteFlag(false);

        static float lastTemp = -1000.0f;
        static float lastSoilHum = -1.0f;
        float temperature = -1000.0f;
        float soilHumidity = -1.0f;

        if (sensorManager.getTempSensor(temp_channel_t::TEMPERATURE_0) !=
            nullptr) {
            temperature =
                sensorManager.getTempSensor(temp_channel_t::TEMPERATURE_0)
                    ->getTemperatureCelsius();
            float dutyCycle =
                ((temperature * 100.0f) / 50.0f) / 100; // Scale 20-40C
            pwm::PWMManager::getInstance().setDutyCycle(
                pwm_channel_t::FAN, dutyCycle); // Control first PWM channel
        }
        if (sensorManager.getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0) !=
            nullptr) {
            soilHumidity =
                sensorManager.getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
                    ->getHumidityPercent();
            float dutyCycle = (100.0f - soilHumidity) / 100.0f; // Scale 0-100%
            pwm::PWMManager::getInstance().setDutyCycle(
                pwm_channel_t::PUMP, dutyCycle); // Control second PWM channel
        }
        if (temperature != lastTemp + 0.5 || temperature != lastTemp - 0.5 ||
            soilHumidity != lastSoilHum + 1.0f ||
            soilHumidity != lastSoilHum - 1.0f) {
            lastSoilHum = soilHumidity;
            lastTemp = temperature;
            print_status(lastTemp, lastSoilHum);
        }
    }

    pwm::PWMManager::getInstance().enableAll(true);
}

void main_serre_init(void) {
    utils::debouncer::init_debouncer();
    i2c::init_i2c();
    lcd::init_lcd();
    pwm::init_pwm();
    adc_manager::init_adc();
    sensor::init_sensors();

    print_welcome_message();
    HAL_Delay(10000);
}

void utils::debouncer::init_debouncer() {
    utils::debouncer::DebouncerHandler debouncer_handlers[] = {
        {PSH_BUT_UP_GPIO_Port, PSH_BUT_UP_Pin, 50, 0, GPIO_PIN_RESET},
        {PSH_BUT_DOWN_GPIO_Port, PSH_BUT_DOWN_Pin, 50, 0, GPIO_PIN_RESET},
        {PSH_BUT_SEL_GPIO_Port, PSH_BUT_SEL_Pin, 50, 0, GPIO_PIN_RESET}};
    utils::debouncer::DebouncerManager::initialize(debouncer_handlers, 3);
}

void i2c::init_i2c(void) {
    i2c::i2c_handler_t i2c_handler = {&hi2c1, 0x48 << 1, 1000, 4};
    i2c::I2CManager::initialize(&i2c_handler, 1);
}

void lcd::init_lcd(void) {
    lcd::gpio_pin_handler_t gpio_data[4] = {{DATA0_GPIO_Port, DATA0_Pin},
                                            {DATA1_GPIO_Port, DATA1_Pin},
                                            {DATA2_GPIO_Port, DATA2_Pin},
                                            {DATA3_GPIO_Port, DATA3_Pin}};
    lcd::gpio_pin_handler_t gpio_rs = {RS_GPIO_Port, RS_Pin};
    lcd::gpio_pin_handler_t gpio_enable = {EN_GPIO_Port, EN_Pin};
    lcd::lcd_handler_t lcd_handler = {
        .i2c_handler = &(i2c::I2CManager::getInstance()),
        .i2c_channel = i2c_channel_t::I2C_CHANNEL_0,
        .mode = lcd::LCD_MODE_I2C,
        .pin_config = {.data_pins = {gpio_data[0], gpio_data[1], gpio_data[2],
                                     gpio_data[3]},
                       .rs = gpio_rs,
                       .enable = gpio_enable}};
    lcd::LCD::initialize(lcd_handler);
}

void pwm::init_pwm(void) {
    pwm::pwm_handler_t pwm_handlers[] = {{&htim1, TIM_CHANNEL_1},
                                         {&htim1, TIM_CHANNEL_2}

    };
    pwm::PWMManager::initialize(pwm_handlers, 2);
}

void adc_manager::init_adc(void) {
    adc_manager::ADCManager::initialize(&hadc1, &hdma_adc1, &htim2);
    if (HAL_OK != ADCManager::getInstance().start()) {
        Error_Handler();
    }
}

void sensor::init_sensors(void) {
    static sensor::TempSensor tempSensor0(
        &adc_manager::ADCManager::getInstance(), 1); // Channel 1
    static sensor::SoilHumSensor soilHumSensor0(
        &adc_manager::ADCManager::getInstance(), 0); // Channel 0

    sensor::SensorManager::getInstance().subscribeTempSensor(&tempSensor0);
    sensor::SensorManager::getInstance().subscribeSoilHumSensor(
        &soilHumSensor0);
}

void calibrate_sensors(void) {
    sensor::SensorManager &sensorManager = sensor::SensorManager::getInstance();
    sensorManager.getTempSensor(temp_channel_t::TEMPERATURE_0)
        ->calibrateOffset();
    sensorManager.getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
        ->calibrateOffset();
}
