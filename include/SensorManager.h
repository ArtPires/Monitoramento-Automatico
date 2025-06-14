#ifndef _SENSOR_MANAGER_H_
#define _SENSOR_MANAGER_H_

#include <array>
#include <cstring>

#include "Commons.h"

#ifdef __aarch64__ 
#include "WaterLevelSensor.h"
#include "MoistureSensor.h"
#else
#include "WaterLevelSensorMock.h"
#include "MoistureSensorMock.h"
#endif

#define NUMBER_OF_SENSORS 2 

typedef struct __attribute__((packed)) {
    char sensorName[32];
    SystemStatus status;
    uint64_t lastTimeOk;
} SensorStatus;

class SensorManager {
public:
    SensorManager();
    ~SensorManager();
    void ConfigureSensors();
    uint16_t readMoisture();
    uint16_t readWaterLevel();
    std::array<SensorStatus, NUMBER_OF_SENSORS> GetAllSensorsStatus();
    SensorStatus GetMoistureSensorStatus();
    SensorStatus GetWaterLevelSensorStatus();
private:
    std::array<SensorStatus, NUMBER_OF_SENSORS> allSensorsStatus_;

#ifdef __aarch64__ 
    std::shared_ptr<MoistureSensor> moistureSensor_;
    std::shared_ptr<WaterLevelSensor> waterLevelSensor_;
#else
    std::shared_ptr<MoistureSensorMock> moistureSensor_;
    std::shared_ptr<WaterLevelSensorMock> waterLevelSensor_;
#endif
};

#endif //_SENSOR_MANAGER_H_