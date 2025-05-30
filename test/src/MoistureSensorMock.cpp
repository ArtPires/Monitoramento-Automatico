#include "MoistureSensorMock.h"

MoistureSensorMock::MoistureSensorMock() {
    mockMoistureSensorStatus_ = SystemStatus::STARTING;
};

MoistureSensorMock::~MoistureSensorMock() {
    mockMoistureSensorStatus_ = SystemStatus::ERROR;
};

void MoistureSensorMock::configureSensor() {
    Log::info("MockMoistureSensor Configured");
    mockMoistureSensorStatus_ = SystemStatus::RUNNING;
};

uint8_t MoistureSensorMock::readData() {
    uint8_t data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 254);
    data = static_cast<uint8_t>(dist(gen));
    Log::info("MockMoistureSensor data: " + data);
    return data;
};

SystemStatus MoistureSensorMock::getStatus() {
    return mockMoistureSensorStatus_;
};