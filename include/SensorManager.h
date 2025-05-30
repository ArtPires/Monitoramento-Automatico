#ifndef _SENSOR_MANAGER_H_
#define _SENSOR_MANAGER_H_

#ifdef __aarch64__ 
#include <wiringPi.h>
#else
#include "WaterPumpMock.h"
#endif

#include "Commons.h"

enum SensorType : uint8_t {
    INPUT_SENSOR,
    OUTPUT_SENSOR
};

typedef struct __attribute__((packed)) {
    SystemStatus status;
    uint64_t lastTimeOk;
} SensorStatus;

typedef struct __attribute__((packed)) {
    uint8_t pin;
    SensorType type;
} SensorConfig;

class SensorManager {
public:
    SensorManager();
    ~SensorManager();
    
    void ConfigSensors();
    uint8_t ReadData(uint8_t sensor);
    SensorStatus GetSensorStatus();
    void UpdateSensorStatus(SensorStatus status);
private:
    SensorStatus sensorStatus_;
    std::string moistureSensor;
    std::string waterLevelSensor;
};

#endif //_SENSOR_MANAGER_H_