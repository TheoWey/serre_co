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

namespace sensor {

SensorManager::SensorManager() {
}

SensorManager::~SensorManager() {
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

void SensorManager::readAllSensors() {
    this->readTempData();
    this->readSoilHumData();
}

void SensorManager::processAllSensors() {
    this->processTempData();
    this->processSoilHumData();
}

void SensorManager::updateAllSensors() {
    this->readAllSensors();
    this->processAllSensors();
}

TempSensor *SensorManager::getTempSensor(uint8_t index) {
    if (index < MAX_SENSORS) {
        return this->tempSensor[index];
    }
    // Handle out-of-bounds access as needed (e.g., throw exception or return
    // nullptr)
    return nullptr; // Placeholder return
}

SoilHumSensor *SensorManager::getSoilHumSensor(uint8_t index) {
    if (index < MAX_SENSORS) {
        return this->soilHumSensor[index];
    }
    // Handle out-of-bounds access as needed (e.g., throw exception or return
    // nullptr)
    return nullptr; // Placeholder return
}

} // namespace sensor
