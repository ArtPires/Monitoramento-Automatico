#ifndef _WATER_LEVEL_SENSOR_
#define _WATER_LEVEL_SENSOR_

#include "Commons.h"

class WaterLevelSensor {
public:
    WaterLevelSensor();
    ~WaterLevelSensor();
    virtual void configureSensor();
    virtual uint16_t readData();
    virtual SystemStatus getStatus();
private:
    SystemStatus waterLevelSensorStatus_;
};

#endif //_WATER_LEVEL_SENSOR_