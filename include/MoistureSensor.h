#ifndef _MOISTURE_SENSOR_
#define _MOISTURE_SENSOR_

#include "Commons.h"

class MoistureSensor {
public:
    MoistureSensor();
    ~MoistureSensor();
    virtual void configureSensor();
    virtual uint16_t readData();
    virtual SystemStatus getStatus();
private:
    SystemStatus moistureSensorStatus_;
};


#endif //_MOISTURE_SENSOR_