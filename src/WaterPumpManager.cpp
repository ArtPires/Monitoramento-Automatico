#include "WaterPumpManager.h"

WaterPumpManager::WaterPumpManager(){
    Log::info("Starting WaterPumpManager...");
#ifdef __aarch64__
    waterPump_ = std::make_shared<WaterPump>();
#else
    waterPump_ = std::make_shared<WaterPumpMock>();
    Log::debug("Using WaterPump in Mock mode!");
#endif
};

WaterPumpManager::~WaterPumpManager(){
    Log::info("WaterPumpManager Stopped!");
};

void WaterPumpManager::activate(){
    waterPump_->activate();
};

void WaterPumpManager::deactivate(){
    waterPump_->deactivate();
};

WaterPumpStatus WaterPumpManager::GetWaterPumpStatus(){
    SystemStatus actualStatus = waterPump_->getStatus();
    waterPumpStatus_.status = actualStatus;

    if (actualStatus == SystemStatus::RUNNING){
        waterPumpStatus_.lastTimeOk = getTimeNow();
    }

    return waterPumpStatus_;
};