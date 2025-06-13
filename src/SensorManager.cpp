#include "SensorManager.h"

SensorManager::SensorManager() {
    Log::info("Starting SensorManager...");    
#ifdef __aarch64__
    moistureSensor_ = std::make_shared<MoistureSensor>();
    waterLevelSensor_ = std::make_shared<WaterLevelSensor>();
#else
    moistureSensor_ = std::make_shared<MoistureSensorMock>();
    waterLevelSensor_ = std::make_shared<WaterLevelSensorMock>();
    Log::debug("Using sensors in Mock mode!");
#endif    
};

SensorManager::~SensorManager() {
    Log::info("SensorManager Stopped!");
}

void SensorManager::ConfigureSensors() {
    moistureSensor_->configureSensor();
    waterLevelSensor_->configureSensor();
};

uint16_t SensorManager::readMoisture() {
    return moistureSensor_->readData();
};

uint16_t SensorManager::readWaterLevel() {
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