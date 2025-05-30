#include "WaterPumpManager.h"

WaterPumpManager::WaterPumpManager(){
#ifdef __aarch64__
    waterPump_ = new WaterPump();
#else
    waterPump_ = new WaterPumpMock();
    Log::debug("Using WaterPump in Mock mode!");
#endif
};

WaterPumpManager::~WaterPumpManager(){
    delete (waterPump_);
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