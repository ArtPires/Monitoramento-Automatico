#ifndef _WATER_LEVEL_SENSOR_MOCK
#define _WATER_LEVEL_SENSOR_MOCK

#include <random>

#include "Commons.h"

class WaterLevelSensorMock {
public:
    WaterLevelSensorMock();
    ~WaterLevelSensorMock();
    void configureSensor();
    uint8_t readData();
    SystemStatus getStatus();
private:
    SystemStatus mockWaterLevelSensorStatus_;
};


#endif //_WATER_LEVEL_SENSOR_MOCK