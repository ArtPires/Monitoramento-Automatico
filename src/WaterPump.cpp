#include <unistd.h>

#include "WaterPump.h"

WaterPump::WaterPump(){
    wiringPiSetup();
    pinMode(WATER_PUMP, OUTPUT);
    waterPumpStatus_ = SystemStatus::RUNNING;
    Log::info("WATER_PUMP configured at pin " + get_physical_pin(WATER_PUMP));
};

WaterPump::~WaterPump(){
    waterPumpStatus_ = SystemStatus::ERROR;
};

void WaterPump::activate(){
    digitalWrite(WATER_PUMP, ACTIVATE);
    Log::info("WATER_PUMP activated for 1 second!");
    sleep(1);
    digitalWrite(WATER_PUMP, DEACTIVATE);
    Log::info("WATER_PUMP deactivated!");
};

void WaterPump::deactivate(){
    digitalWrite(WATER_PUMP, DEACTIVATE);
    Log::info("WATER_PUMP DEACTIVATE!");
};

SystemStatus WaterPump::getStatus(){
    return waterPumpStatus_;
};