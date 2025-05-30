#include "WaterPumpMock.h"

WaterPumpMock::WaterPumpMock() {
    mockWaterPumpStatus_ = SystemStatus::RUNNING;
    Log::debug("mockWaterPumpStatus_: " + std::to_string(mockWaterPumpStatus_));
};

WaterPumpMock::~WaterPumpMock() {
    mockWaterPumpStatus_ = SystemStatus::ERROR;
    Log::debug("mockWaterPumpStatus_: " + std::to_string(mockWaterPumpStatus_));
};

void WaterPumpMock::activate() {
    Log::info("MockWaterPump ACTIVATE");
};

void WaterPumpMock::deactivate() {
    Log::info("MockWaterPump DEACTIVATE");
};

SystemStatus WaterPumpMock::getStatus() {
    return mockWaterPumpStatus_;
};