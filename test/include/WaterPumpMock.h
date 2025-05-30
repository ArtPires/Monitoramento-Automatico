#include "Commons.h"
#include "WaterPump.h"

class WaterPumpMock : WaterPump{
public:
    WaterPumpMock();
    ~WaterPumpMock();
    void activate();
    void deactivate();
private:
    SystemStatus mockWaterPumpStatus_;
};