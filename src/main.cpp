#include <iostream>
#include <unistd.h>

#include "Commons.h"
#include "SensorManager.h"

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

    parseArgs(argc, argv);

    Log::setLogFile(LOG_FILE_PATH);

    SensorManager sensorManager;
    sensorManager.ConfigSensors();

    if (sensorManager.GetSensorStatus().status == SystemStatus::RUNNING){
        systemStatus_ = SystemStatus::RUNNING;
    }

    while (systemStatus_ == SystemStatus::RUNNING)
    {
        int read = sensorManager.ReadData(SOIL_MOISTURE_SENSOR);

        if (read == 0){
            sensorManager.WriteData(WATER_PUMP, ACTIVATE);
        } else {
            sensorManager.WriteData(WATER_PUMP, DEACTIVATE);
        }
        usleep(1000000);
    }
    
    return 0;
}
