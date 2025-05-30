#include "WaterLevelSensorMock.h"

WaterLevelSensorMock::WaterLevelSensorMock() {
    mockWaterLevelSensorStatus_ = SystemStatus::STARTING;
    Log::debug("mockWaterLevelSensorStatus_: " + std::to_string(mockWaterLevelSensorStatus_));

};

WaterLevelSensorMock::~WaterLevelSensorMock() {
    mockWaterLevelSensorStatus_ = SystemStatus::ERROR;
    Log::debug("mockWaterLevelSensorStatus_: " + std::to_string(mockWaterLevelSensorStatus_));
};

void WaterLevelSensorMock::configureSensor() {
    Log::info("WaterLevelSensor Configured");
};

uint8_t WaterLevelSensorMock::readData() {
    uint8_t data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 254);
    data = static_cast<uint8_t>(dist(gen));
    Log::debug("WaterLevelSensorMock data: " + std::to_string(data));
    return data;
};

SystemStatus WaterLevelSensorMock::getStatus() {
    return mockWaterLevelSensorStatus_;
};