#include <iostream>
#include <unistd.h>
#include <backward.hpp>

#include "Commons.h"
#include "SensorManager.h"
#include "WaterPumpManager.h"
#include "TCPServer.h"
#include "ads1115.h"

#define NO_ARGS 1

void parseArgs(int argc, char* argv[]) {
    int opt;

    if (argc == NO_ARGS) { return; }

    opt = getopt(argc, argv, "d");
    switch (opt) {
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

    auto waterPumpManager = std::make_unique<WaterPumpManager>();

    auto sensorManager = std::make_unique<SensorManager>();
    //sensorManager->ConfigureSensors();
    
    systemStatus_ = SystemStatus::RUNNING;
    
    //for (auto& s : sensorManager->GetAllSensorsStatus()) {
    //    if (s.status != SystemStatus::RUNNING) {
    //        systemStatus_ = s.status;
    //    }
    //}

    auto commandHandler = [&waterPumpManager](const std::string& cmd) {
        if (cmd == "activate") {
            waterPumpManager->activate();
        } else if (cmd == "deactivate") {
            waterPumpManager->deactivate();
        } else if (cmd == "stop") {
            systemStatus_ = SystemStatus::STOPPING;
        } else {
            Log::warning("Unknown command: " + cmd);
        }
    };

    TcpServer server(8080, commandHandler);
    server.start();

    ADS1115 adc;

    while (systemStatus_ == SystemStatus::RUNNING) {
        int soil = adc.readChannel(0);    // Sensor umidade (AIN0)
        int water = adc.readChannel(1);   // Sensor bóia (AIN1)

        std::cout << "Umidade do solo: " << soil << std::endl;
        std::cout << "Nível da água: " << water << std::endl;
        
        //uint8_t moisture = sensorManager->readMoisture();
        //uint8_t waterLevel = sensorManager->readWaterLevel();

        //if (moisture >= 127 && waterLevel >= 127) {
        //    waterPumpManager->activate();
        //} else {
        //    waterPumpManager->deactivate();
        //}
        sleep(1);
    }

    Log::info("SystemStatus: " + std::to_string(systemStatus_));
    Log::info("Finishing AutoMonitor...");

    return 0;
}
