#include "WaterLevelSensorMock.h"

WaterLevelSensorMock::WaterLevelSensorMock() {
    mockWaterLevelSensorStatus_ = SystemStatus::RUNNING;
};

WaterLevelSensorMock::~WaterLevelSensorMock() {
    mockWaterLevelSensorStatus_ = SystemStatus::ERROR;
};

void WaterLevelSensorMock::configureSensor() {
    Log::info("WaterLevelSensor ACTIVATE");
};

uint8_t WaterLevelSensorMock::readData() {
    uint8_t data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 254);
    data = static_cast<uint8_t>(dist(gen));
    Log::info("WaterLevelSensorMock data: " + data);
    return data;
};

SystemStatus WaterLevelSensorMock::getStatus() {
    return mockWaterLevelSensorStatus_;
};