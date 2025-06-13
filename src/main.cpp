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

json buildSystemStatusJson(SystemStatus systemStatus,
                           uint16_t moisture, uint16_t waterLevel,
                           const WaterPumpStatus& waterPump,
                           const SensorStatus& moistureSensor,
                           const SensorStatus& waterLevelSensor) {
    json root;

    root["systemStatus"] = {
        {"status", status_to_string(systemStatus)},
        {"moisture", moisture},
        {"waterLevel", waterLevel}
    };

    root["waterPump"] = {
        {"status", status_to_string(waterPump.status)},
        {"lastTimeOk", waterPump.lastTimeOk}
    };

    root["moistureSensor"] = {
        {"name", moistureSensor.sensorName},
        {"status", status_to_string(moistureSensor.status)},
        {"lastTimeOk", moistureSensor.lastTimeOk}
    };

    root["waterLevelSensor"] = {
        {"name", waterLevelSensor.sensorName},
        {"status", status_to_string(waterLevelSensor.status)},
        {"lastTimeOk", waterLevelSensor.lastTimeOk}
    };

    return root;
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

    auto commandHandler = [&waterPumpManager, &sensorManager](const std::string& cmd) -> std::string {
        if (cmd == "activate") {
            waterPumpManager->activate();
            return "Water Pump activated!";
        } else if (cmd == "deactivate") {
            waterPumpManager->deactivate();
            return "Water Pump deactivated!";
        } else if (cmd == "reboot") {
            systemStatus_ = SystemStatus::STOPPING;
            return "Rebooting system...";
        } else if (cmd == "status") {
            uint16_t moisture = sensorManager->readMoisture();
            usleep(150000);
            uint16_t waterLevel = sensorManager->readWaterLevel();

            WaterPumpStatus waterPumpStatus = waterPumpManager->GetWaterPumpStatus();
            SensorStatus moistureStatus = sensorManager->GetMoistureSensorStatus();
            SensorStatus waterLevelStatus = sensorManager->GetWaterLevelSensorStatus();

            json statusJson = buildSystemStatusJson(systemStatus_, moisture, waterLevel, waterPumpStatus, moistureStatus, waterLevelStatus);
            return statusJson.dump(4);
        } else {
            Log::warning("Unknown command: " + cmd);
            return "Unknown command: " + cmd;
        }
        return "";
    };

    TcpServer server(8080, commandHandler);
    server.start();

    while (systemStatus_ == SystemStatus::RUNNING) {
        uint16_t moisture = sensorManager->readMoisture();
        usleep(150000);
        uint16_t water = sensorManager->readWaterLevel();

        Log::debug("Moisture Level: " + std::to_string(moisture));
        Log::debug("Water Level: " + std::to_string(water));

        if (moisture >= systemConfig.moisture_treshold && water <= systemConfig.water_level_treshold) {
            waterPumpManager->activate();
        }
        
        sleep(1);
    }

    Log::info("SystemStatus: " + status_to_string(systemStatus_));
    Log::info("Finishing AutoMonitor...");

    return 0;
}
