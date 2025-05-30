#ifndef _WATER_PUMP_H_
#define _WATER_PUMP_H_

#include <wiringPi.h>

#include "Commons.h"

class WaterPump {
public:
    WaterPump();
    ~WaterPump();
    virtual void activate();
    virtual void deactivate();
    virtual SystemStatus getStatus();
private:
    SystemStatus waterPumpStatus_;
};

#endif // _WATER_PUMP_H_