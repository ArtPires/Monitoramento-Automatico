#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor() {
    waterLevelSensorStatus_ = SystemStatus::STARTING;
};

WaterLevelSensor::~WaterLevelSensor() {
    waterLevelSensorStatus_ == SystemStatus::ERROR;
};

void WaterLevelSensor::configureSensor(){
    wiringPiSetup();
    pinMode(WATER_LEVEL_SENSOR, INPUT);
    waterLevelSensorStatus_ = SystemStatus::RUNNING;
    Log::info("WATER_LEVEL_SENSOR configured at pin " + get_physical_pin(WATER_LEVEL_SENSOR));
};

uint8_t WaterLevelSensor::readData() {
    return digitalRead(WATER_LEVEL_SENSOR);
};

SystemStatus WaterLevelSensor::getStatus() {
    return waterLevelSensorStatus_;
};