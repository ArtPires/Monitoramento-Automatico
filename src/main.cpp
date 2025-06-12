#include <iostream>
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

int read_sensor(const char* device_path) {
    int fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Erro ao abrir " << device_path << ": " << strerror(errno) << std::endl;
        return -1;
    }

    char buf[32];
    ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
    if (bytes_read < 0) {
        std::cerr << "Erro ao ler " << device_path << ": " << strerror(errno) << std::endl;
        close(fd);
        return -1;
    }

    buf[bytes_read] = '\0';

    int value = std::stoi(buf); 

    close(fd);
    return value;
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

    while (systemStatus_ == SystemStatus::RUNNING) {
        int soil = read_sensor("/dev/soil_sensor");    // Sensor umidade (AIN0)
        int water = read_sensor("/dev/water_level_sensor");   // Sensor bóia (AIN1)

        std::cout << "Umidade do solo: " << soil << std::endl;
        std::cout << "Nível da água: " << water << std::endl;

        if (soil >= 17000 && water <= 20000) {
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
