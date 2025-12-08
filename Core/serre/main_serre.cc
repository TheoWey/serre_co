#include "main_serre.h"

#include "UTILS/menu_controler/inc/menu_controler.hh"

#include "UTILS/debouncer/inc/debouncer.hh"
#include "driver/ADC_MANAGER/inc/adc_manager.hh"
#include "driver/I2C/inc/i2c_manager.hh"
#include "driver/LCD/inc/lcd.hh"
#include "driver/PWM/inc/pwm_manager.hh"
#include "driver/sensors/inc/sensor_manager.hh"
#include "utils/mae_serre/inc/mae_serre.hh"

void main_serre(void) {
    utils::ui::UIControler::getInstance().update();
    utils::mae_serre::SerreController::getInstance().processLoop();
    uint32_t currentTick = HAL_GetTick();
    static uint32_t lastCalibTick = 0;
    if ((currentTick - lastCalibTick) >= 1000) {
        lastCalibTick = currentTick;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
}

void main_serre_init(void) {
    utils::debouncer::init_debouncer();
    driver::i2c::init_i2c();
    driver::lcd::init_lcd();
    driver::pwm::init_pwm();
    driver::adc_manager::init_adc();
    driver::sensor::init_sensors();
}

void utils::debouncer::init_debouncer() {
    utils::debouncer::DebouncerHandler debouncer_handlers[] = {
        {PSH_BUT_UP_GPIO_Port, PSH_BUT_UP_Pin, 50, 2000},
        {PSH_BUT_DOWN_GPIO_Port, PSH_BUT_DOWN_Pin, 50, 2000},
        {PSH_BUT_SEL_GPIO_Port, PSH_BUT_SEL_Pin, 50, 2000}};
    utils::debouncer::DebouncerManager::initialize(debouncer_handlers, 3);
}

void driver::i2c::init_i2c(void) {
    driver::i2c::i2c_handler_t i2c_handler = {&hi2c1, 0x27, 1000, 4};
    driver::i2c::I2CManager::initialize(&i2c_handler, 1);
}

void driver::lcd::init_lcd(void) {
    driver::lcd::gpio_pin_handler_t gpio_data[4] = {
        {DATA0_GPIO_Port, DATA0_Pin},
        {DATA1_GPIO_Port, DATA1_Pin},
        {DATA2_GPIO_Port, DATA2_Pin},
        {DATA3_GPIO_Port, DATA3_Pin}};
    driver::lcd::gpio_pin_handler_t gpio_rs = {RS_GPIO_Port, RS_Pin};
    driver::lcd::gpio_pin_handler_t gpio_enable = {EN_GPIO_Port, EN_Pin};
    driver::lcd::lcd_handler_t lcd_handler = {
        .i2c_handler = &(driver::i2c::I2CManager::getInstance()),
        .i2c_channel = i2c_channel_t::I2C_CHANNEL_0,
        .mode = driver::lcd::LCD_MODE_I2C,
        .pin_config = {.data_pins = {gpio_data[0], gpio_data[1], gpio_data[2],
                                     gpio_data[3]},
                       .rs = gpio_rs,
                       .enable = gpio_enable}};
    driver::lcd::LCD::initialize(lcd_handler);
    utils::ui::UIControler::getInstance().init();
}

void driver::pwm::init_pwm(void) {
    driver::pwm::pwm_handler_t pwm_handlers[] = {{&htim1, TIM_CHANNEL_1},
                                                 {&htim1, TIM_CHANNEL_2}

    };
    driver::pwm::PWMManager::initialize(pwm_handlers, 2);
}

void driver::adc_manager::init_adc(void) {
    driver::adc_manager::ADCManager::initialize(&hadc1, &hdma_adc1, &htim2);
    if (HAL_OK != driver::adc_manager::ADCManager::getInstance().start()) {
        Error_Handler();
    }
}

void driver::sensor::init_sensors(void) {
    static driver::sensor::TempSensor tempSensor0(
        &adc_manager::ADCManager::getInstance(), 1); // Channel 1
    static driver::sensor::SoilHumSensor soilHumSensor0(
        &adc_manager::ADCManager::getInstance(), 0); // Channel 0

    driver::sensor::SensorManager::getInstance().subscribeTempSensor(
        &tempSensor0);
    driver::sensor::SensorManager::getInstance().subscribeSoilHumSensor(
        &soilHumSensor0);
}

void calibrate_sensors(void) {
    driver::sensor::SensorManager &sensorManager =
        driver::sensor::SensorManager::getInstance();
    sensorManager.getTempSensor(temp_channel_t::TEMPERATURE_0)
        ->calibrateOffset();
    sensorManager.getSoilHumSensor(hum_channel_t::SOIL_HUMIDITY_0)
        ->calibrateOffset();
}
