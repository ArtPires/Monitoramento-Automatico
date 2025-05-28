#ifndef _WATER_PUMP_MANAGER_H_
#define _WATER_PUMP_MANAGER_H_

#ifdef __aarch64__ 
#include <wiringPi.h>
#else
#include "WaterPumpMock.h"
#endif

#include "Commons.h"

typedef struct __attribute__((packed)) {
    SystemStatus status;
    uint32_t lastTimeOk;
} WaterPumpStatus;

class WaterPumpManager {
public:
    WaterPumpManager();
    ~WaterPumpManager();
    void activate();
    void deactivate();
    WaterPumpStatus GetWaterPumpStatus();
    void UpdateWaterPumpStatus(WaterPumpStatus status);
private:
    WaterPumpStatus waterPumpStatus_;
    
#ifdef __aarch64__ 
    
#else
    WaterPumpMock* waterPump_;
#endif

};

#endif //_WATER_PLUMP_MANAGER_H_