#include <iostream>
#include <unistd.h>
#include <backward.hpp>

#include "Commons.h"
#include "SensorManager.h"
#include "WaterPumpManager.h"
#include "TCPServer.h"

#include <nlohmann/json.hpp>

#define NO_ARGS 1

using json = nlohmann::json;

void getConfigFromFile(SystemConfig* systemConfig){
    std::ifstream arquivo("config_files/config.json");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir config.json\n";
        return;
    }

    json cfg;
    arquivo >> cfg;

    systemConfig->moisture_treshold = cfg["moisture_treshold"];
    systemConfig->water_level_treshold = cfg["water_level_treshold"];
}

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

    SystemConfig systemConfig;
    getConfigFromFile(&systemConfig);

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
        } else if (cmd == "reboot") {
            // TODO(artur.pires): implement reboot
        } else if (cmd == "status") {
            // TODO(artur.pires): implement a function to send all status back to client
        } else {
            Log::warning("Unknown command: " + cmd);
        }
    };

    TcpServer server(8080, commandHandler);
    server.start();

    while (systemStatus_ == SystemStatus::RUNNING) {
        uint16_t moisture = sensorManager->readMoisture();
        usleep(150000);
        uint16_t water = sensorManager->readWaterLevel();

        std::cout << "Umidade do solo: " << moisture << std::endl;
        std::cout << "Nível da água: " << water << std::endl;

        if (moisture >= systemConfig.moisture_treshold && water <= systemConfig.water_level_treshold) {
            waterPumpManager->activate();
        } else {
            waterPumpManager->deactivate();
        }
        sleep(1);
    }

    Log::info("SystemStatus: " + std::to_string(systemStatus_));
    Log::info("Finishing AutoMonitor...");

    return 0;
}
