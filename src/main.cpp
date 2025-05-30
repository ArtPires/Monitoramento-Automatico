#include <iostream>
#include <unistd.h>
#include <backward.hpp>

#include "Commons.h"
#include "SensorManager.h"
#include "WaterPumpManager.h"

#define NO_ARGS 1

void parseArgs(int argc, char* argv[]) {
    int opt;

    if (argc == NO_ARGS){ return; }

    opt = getopt(argc, argv, "d");
    switch (opt)
    {
    case 'd':
        Log::setLogLevel(LogLevel::DEBUG);
        break;
    default:
        Log::error("Parametro invalido! Utilizar [-d] para habilitar o modo debug!");
        break;
    }
    
}

int main(int argc, char* argv[]) {
    backward::SignalHandling sh;

    parseArgs(argc, argv);

    Log::setLogFile(LOG_FILE_PATH);

    WaterPumpManager* waterPumpManager = new WaterPumpManager();
    
    SensorManager* sensorManager = new SensorManager();
    sensorManager->ConfigureSensors();

    systemStatus_ = SystemStatus::RUNNING;
    for (auto& s : sensorManager->GetAllSensorsStatus()) {
        if (s.status != SystemStatus::RUNNING) {
            systemStatus_ == s.status;
        }
    }

    while (systemStatus_ == SystemStatus::RUNNING) {
        uint8_t moisture = sensorManager->readMoisture();
        uint8_t waterLevel = sensorManager->readWaterLevel();

        if (moisture >= 127 && waterLevel >= 127){
            waterPumpManager->activate();
        } else {
            waterPumpManager->deactivate();
        }
        sleep(1);
    }

    delete(waterPumpManager);
    delete(sensorManager);

    Log::info("SystemStatus: " + systemStatus_);
    Log::info("Finishing AutoMonitor...");
    
    return 0;
}
