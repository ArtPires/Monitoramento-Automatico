#ifndef _MOISTURE_SENSOR_MOCK_
#define _MOISTURE_SENSOR_MOCK_

#include <random>

#include "Commons.h"

class MoistureSensorMock {
public:
    MoistureSensorMock();
    ~MoistureSensorMock();
    void configureSensor();
    uint8_t readData();
    SystemStatus getStatus();
private:
    SystemStatus mockMoistureSensorStatus_;
};

#endif //_MOISTURE_SENSOR_MOCK_