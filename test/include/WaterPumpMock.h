#ifndef _WATER_PUMP_MOCK_
#define _WATER_PUMP_MOCK_

#include "Commons.h"

class WaterPumpMock {
public:
    WaterPumpMock();
    ~WaterPumpMock();
    void activate();
    void deactivate();
    SystemStatus getStatus();
private:
    SystemStatus mockWaterPumpStatus_;
};

#endif //_WATER_PUMP_MOCK_