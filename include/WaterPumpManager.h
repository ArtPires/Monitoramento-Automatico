#ifndef _WATER_PUMP_MANAGER_H_
#define _WATER_PUMP_MANAGER_H_

#ifdef __aarch64__ 
#include "WaterPump.h"
#else
#include "WaterPumpMock.h"
#endif

#include "Commons.h"

typedef struct __attribute__((packed)) {
    SystemStatus status;
    uint64_t lastTimeOk;
} WaterPumpStatus;

class WaterPumpManager {
public:
    WaterPumpManager();
    ~WaterPumpManager();
    void activate();
    void deactivate();
    WaterPumpStatus GetWaterPumpStatus();
private:
    WaterPumpStatus waterPumpStatus_;
#ifdef __aarch64__ 
    std::shared_ptr<WaterPump> waterPump_;
#else
    std::shared_ptr<WaterPumpMock> waterPump_;
#endif
};

#endif //_WATER_PUMP_MANAGER_H_