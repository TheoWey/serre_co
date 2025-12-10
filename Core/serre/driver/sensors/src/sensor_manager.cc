/**
 * @file sensor_manager.cc
 * @author ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 * @brief Sensor Manager class for handling multiple sensors.
 * This class provides an interface to manage and coordinate
 * multiple sensors in the system.
 */

#include "../inc/sensor_manager.hh"

using namespace driver::sensor;

__WEAK void init_sensors(void) {
    // Weak implementation for user override
}

void SensorManager::initialize() {
    // Placeholder for future initialization logic
    SensorManager &instance = getInstance();
    (void)instance;
}

SensorManager &SensorManager::getInstance() {
    static SensorManager instance;
    return instance;
}

void SensorManager::subscribeTempSensor(TempSensor *sensor) {
    SensorSubscribe<TempSensor>(this->tempSensor, sensor);
}

void SensorManager::subscribeSoilHumSensor(SoilHumSensor *sensor) {
    SensorSubscribe<SoilHumSensor>(this->soilHumSensor, sensor);
}

void SensorManager::readTempData(uint8_t index) {
    SensorDataOperation<TempSensor>(this->tempSensor, &TempSensor::readData,
                                    index);
}

void SensorManager::readSoilHumData(uint8_t index) {
    SensorDataOperation<SoilHumSensor>(this->soilHumSensor,
                                       &SoilHumSensor::readData, index);
}
void SensorManager::processTempData(uint8_t index) {
    SensorDataOperation<TempSensor>(this->tempSensor, &TempSensor::processData,
                                    index);
}

void SensorManager::processSoilHumData(uint8_t index) {
    SensorDataOperation<SoilHumSensor>(this->soilHumSensor,
                                       &SoilHumSensor::processData, index);
}

void SensorManager::calibrateTempSensor(uint8_t index) {
    SensorDataOperation<TempSensor>(this->tempSensor,
                                    &TempSensor::calibrateOffset, index);
}

void SensorManager::calibrateSoilHumSensor(uint8_t index) {
    SensorDataOperation<SoilHumSensor>(this->soilHumSensor,
                                       &SoilHumSensor::calibrateOffset, index);
}

void SensorManager::calibrateAllSensors() {
    this->calibrateTempSensor();
    this->calibrateSoilHumSensor();
}

void SensorManager::processAllSensors() {
    this->processTempData();
    this->processSoilHumData();
}

void SensorManager::readAllSensors() {
    this->readTempData();
    this->readSoilHumData();
}

void SensorManager::updateAllSensors() {
    this->readAllSensors();
    this->processAllSensors();
}

TempSensor *SensorManager::getTempSensor(uint8_t index) {
    if (index < MAX_SENSORS) {
        return this->tempSensor[index];
    }
    return nullptr;
}

SoilHumSensor *SensorManager::getSoilHumSensor(uint8_t index) {
    if (index < MAX_SENSORS) {
        return this->soilHumSensor[index];
    }
    return nullptr;
}
