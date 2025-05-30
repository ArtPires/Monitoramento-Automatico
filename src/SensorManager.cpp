#include "SensorManager.h"

SensorManager::SensorManager() {
    SensorStatus newStatus;
    newStatus.status = SystemStatus::RUNNING;
    UpdateSensorStatus(newStatus);
    Log::info("Sensores Configurados!");
};

SensorManager::~SensorManager() { };

void SensorManager::ConfigSensors() {
    wiringPiSetup();
    pinMode(SOIL_MOISTURE_SENSOR, INPUT);
    pullUpDnControl(SOIL_MOISTURE_SENSOR, PUD_DOWN);  // Usa resistor pull-down interno
    Log::debug("Pino 13 [SOIL_MOISTURE_SENSOR] configurado como INPUT com resistor pull-down");
};

uint8_t SensorManager::ReadData(uint8_t sensor) {
    uint8_t data;
    #ifdef __aarch64__ 
        data = digitalRead(sensor);
        Log::debug("Dado recebido: " + std::to_string(data));
    #else
        Log::info("Nao foi possivel ler os dados do sensor, executando em ambiente x86_64!");
        data = 0xFF;
    #endif
    return data;
};

SensorStatus SensorManager::GetSensorStatus() {
    return sensorStatus_;
};

void SensorManager::UpdateSensorStatus(SensorStatus status) {
    sensorStatus_ = status;
};