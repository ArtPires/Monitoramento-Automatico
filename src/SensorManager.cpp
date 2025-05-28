#include "SensorManager.h"

SensorManager::SensorManager() {
    SensorStatus newStatus;
    newStatus.status = SystemStatus::RUNNING;
    UpdateSensorStatus(newStatus);
    Log::info("Sensores Configurados!");
};

SensorManager::~SensorManager() {
    delete(waterPump_);
};

void SensorManager::ConfigSensors() {
    #ifdef __aarch64__ 
        wiringPiSetup();
        pinMode(WATER_PUMP, OUTPUT);
        pinMode(SOIL_MOISTURE_SENSOR, INPUT);
        pullUpDnControl(SOIL_MOISTURE_SENSOR, PUD_DOWN);  // Usa resistor pull-down interno
        Log::debug("Pino 11 [WATER_PUMP] configurado como OUTPUT");
        Log::debug("Pino 13 [SOIL_MOISTURE_SENSOR] configurado como INPUT com resistor pull-down");
    #else
        waterPump_ = new WaterPumpMock (SystemStatus::RUNNING);
        Log::info("Utilizando sensores em modo Mock!");
    #endif
};

void SensorManager::WriteData(uint8_t sensor, uint8_t data) {
    Log::debug("Dado enviado: " + std::to_string(data));
    #ifdef __aarch64__ 
        digitalWrite(sensor, data);
    #else
        waterPump_->digitalWrite(data);
    #endif
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