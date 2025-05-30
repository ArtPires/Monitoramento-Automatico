#include "WaterPumpMock.h"

WaterPumpMock::WaterPumpMock() {
    mockWaterPumpStatus_ = SystemStatus::RUNNING;
};

WaterPumpMock::~WaterPumpMock() {
    mockWaterPumpStatus_ = SystemStatus::ERROR;
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