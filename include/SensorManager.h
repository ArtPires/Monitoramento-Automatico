#ifndef _SENSOR_MANAGER_H_
#define _SENSOR_MANAGER_H_

#ifdef __aarch64__ 
#include <wiringPi.h>
#else
#include "WaterPumpMock.h"
#endif

#include "Commons.h"

#define WATER_PUMP 0 // GPIO WiringPi 0 | Raspberry Pi pin 11
#define SOIL_MOISTURE_SENSOR 2 // GPIO WiringPi 2 | Raspberry Pi pin 13

enum SensorType : uint8_t {
    INPUT_SENSOR,
    OUTPUT_SENSOR
};

typedef struct __attribute__((packed)) {
    SystemStatus status;
    uint32_t lastTimeOk;
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
    void WriteData(uint8_t sensor, uint8_t data);
    uint8_t ReadData(uint8_t sensor);
    SensorStatus GetSensorStatus();
    void UpdateSensorStatus(SensorStatus status);
private:
    SensorStatus sensorStatus_;
    std::string moistureSensor;
    std::string waterLevelSensor;
    WaterPumpMock* waterPump_;
};

#endif //_SENSOR_MANAGER_H_