#include "Commons.h"

class WaterPumpMock {
public:
    WaterPumpMock(SystemStatus initialState);
    ~WaterPumpMock();

    void digitalWrite(uint8_t data);
private:
    SystemStatus waterPumpState_;
    uint8_t currentData_;
};