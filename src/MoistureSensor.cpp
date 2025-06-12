#include "MoistureSensor.h"

MoistureSensor::MoistureSensor() {
    moistureSensorStatus_ = SystemStatus::STARTING;
};

MoistureSensor::~MoistureSensor() {
    moistureSensorStatus_ = SystemStatus::ERROR;
};

void MoistureSensor::configureSensor(){
    wiringPiSetup();
    pinMode(MOISTURE_SENSOR, INPUT);
    moistureSensorStatus_ = SystemStatus::RUNNING;
    Log::info("MOISTURE_SENSOR configured at pin " + get_physical_pin(MOISTURE_SENSOR));
};

uint8_t MoistureSensor::readData() {
    return digitalRead(MOISTURE_SENSOR);
};

SystemStatus MoistureSensor::getStatus() {
    return moistureSensorStatus_;
};