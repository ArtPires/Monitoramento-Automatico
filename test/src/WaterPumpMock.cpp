#include "WaterPumpMock.h"

WaterPumpMock::WaterPumpMock(SystemStatus initialState) {
    waterPumpState_ = initialState;
};

WaterPumpMock::~WaterPumpMock(){
    waterPumpState_ = SystemStatus::ERROR;
};

void WaterPumpMock::digitalWrite(uint8_t data) {
    currentData_ = data;
};