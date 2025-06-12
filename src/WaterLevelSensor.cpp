#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor() {
    waterLevelSensorStatus_ = SystemStatus::STARTING;
};

WaterLevelSensor::~WaterLevelSensor() {
    waterLevelSensorStatus_ == SystemStatus::ERROR;
};

void WaterLevelSensor::configureSensor() {
    struct stat info;
    if (!stat(WATER_LEVEL_SENSOR, &info)) {
        waterLevelSensorStatus_ = SystemStatus::ERROR;
        Log::error("Could not locate " + std::string(WATER_LEVEL_SENSOR));
        return;
    }

    waterLevelSensorStatus_ = SystemStatus::RUNNING;
    Log::info("WATER_LEVEL_SENSOR detected and connected at: " + std::string(WATER_LEVEL_SENSOR));
};

int WaterLevelSensor::readData() {
    int fd = open(WATER_LEVEL_SENSOR, O_RDONLY);
    if (fd < 0) {
        Log::error("Could not open " + std::string(WATER_LEVEL_SENSOR));
        return -1;
    }

    char buf[32];
    ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
    if (bytes_read < 0) {
        Log::error("Could not read " + std::string(WATER_LEVEL_SENSOR));
        close(fd);
        return -1;
    }

    buf[bytes_read] = '\0';

    int value = std::stoi(buf); 

    close(fd);
    return value;
};

SystemStatus WaterLevelSensor::getStatus() {
    return waterLevelSensorStatus_;
};