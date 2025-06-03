#include <iostream>
#include <memory>
#include <unistd.h>
#include <backward.hpp>

#include "Commons.h"
#include "SensorManager.h"
#include "WaterPumpManager.h"
#include "TCPServer.h"

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
    sensorManager->ConfigureSensors();
    
    systemStatus_ = SystemStatus::RUNNING;
    
    for (auto& s : sensorManager->GetAllSensorsStatus()) {
        if (s.status != SystemStatus::RUNNING) {
            systemStatus_ = s.status;
        }
    }

    auto commandHandler = [&waterPumpManager](const std::string& cmd) {
        if (cmd == "activate") {
            waterPumpManager->activate();
        } else if (cmd == "deactivate") {
            waterPumpManager->deactivate();
        } else {
            std::cout << "Comando desconhecido: " << cmd << std::endl;
        }
    };

    TcpServer server(8080, commandHandler);
    server.start();

    while (systemStatus_ == SystemStatus::RUNNING) {
        uint8_t moisture = sensorManager->readMoisture();
        uint8_t waterLevel = sensorManager->readWaterLevel();

        if (moisture >= 127 && waterLevel >= 127) {
            //waterPumpManager->activate();
        } else {
            //waterPumpManager->deactivate();
        }
        sleep(1);
    }

    Log::info("SystemStatus: " + std::to_string(systemStatus_));
    Log::info("Finishing AutoMonitor...");

    return 0;
}
