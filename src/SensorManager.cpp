#include "SensorManager.h"

SensorManager::SensorManager() {
#ifdef __aarch64__
    moistureSensor_ = new MoistureSensor();
    waterLevelSensor_ = new WaterLevelSensor();
#else
    moistureSensor_ = new MoistureSensorMock();
    waterLevelSensor_ = new WaterLevelSensorMock();
    Log::debug("Using sensors in Mock mode!");
#endif    
};

SensorManager::~SensorManager() { 
    delete(moistureSensor_);
    delete(waterLevelSensor_);
};

void SensorManager::ConfigureSensors() {
    moistureSensor_->configureSensor();
    waterLevelSensor_->configureSensor();
};

uint8_t SensorManager::readMoisture() {
    return moistureSensor_->readData();
};

uint8_t SensorManager::readWaterLevel() {
    return waterLevelSensor_->readData();
};

std::array<SensorStatus, NUMBER_OF_SENSORS> SensorManager::GetAllSensorsStatus() {
    strncpy(allSensorsStatus_[0].sensorName, "MOISTURE_SENSOR", sizeof(allSensorsStatus_[0].sensorName));
    allSensorsStatus_[0].status = moistureSensor_->getStatus();
    if (moistureSensor_->getStatus() == SystemStatus::RUNNING){
        allSensorsStatus_[0].lastTimeOk = getTimeNow();
    }

    strncpy(allSensorsStatus_[1].sensorName, "WATER_LEVEL_SENSOR", sizeof(allSensorsStatus_[0].sensorName));
    allSensorsStatus_[1].status = waterLevelSensor_->getStatus();
    if (waterLevelSensor_->getStatus() == SystemStatus::RUNNING){
        allSensorsStatus_[1].lastTimeOk = getTimeNow();
    }

    return allSensorsStatus_;
};

SensorStatus SensorManager::GetMoistureSensorStatus() {
    SensorStatus moistureSensorStatus;
    strncpy(moistureSensorStatus.sensorName, "MOISTURE_SENSOR", sizeof(allSensorsStatus_[0].sensorName));
    moistureSensorStatus.status = moistureSensor_->getStatus();
    if (moistureSensor_->getStatus() == SystemStatus::RUNNING){
        moistureSensorStatus.lastTimeOk = getTimeNow();
    }

    return moistureSensorStatus;
};

SensorStatus SensorManager::GetWaterLevelSensorStatus() {
    SensorStatus waterLevelSensorStatus;
    strncpy(waterLevelSensorStatus.sensorName, "WATER_LEVEL_SENSOR", sizeof(allSensorsStatus_[0].sensorName));
    waterLevelSensorStatus.status = waterLevelSensor_->getStatus();
    if (waterLevelSensor_->getStatus() == SystemStatus::RUNNING){
        waterLevelSensorStatus.lastTimeOk = getTimeNow();
    }

    return waterLevelSensorStatus;
};