#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "MoistureSensor.h"

MoistureSensor::MoistureSensor() {
    moistureSensorStatus_ = SystemStatus::STARTING;
};

MoistureSensor::~MoistureSensor() {
    moistureSensorStatus_ = SystemStatus::ERROR;
};

void MoistureSensor::configureSensor() {
    struct stat info;
    if (!stat(MOISTURE_SENSOR, &info)) {
        moistureSensorStatus_ = SystemStatus::ERROR;
        Log::error("Could not locate " + std::string(MOISTURE_SENSOR));
        return;
    }

    moistureSensorStatus_ = SystemStatus::RUNNING;
    Log::info("MOISTURE_SENSOR detected and connected at: " + std::string(MOISTURE_SENSOR));
};

int MoistureSensor::readData() {
    int fd = open(MOISTURE_SENSOR, O_RDONLY);
    if (fd < 0) {
        Log::error("Could not open " + std::string(MOISTURE_SENSOR));
        return -1;
    }

    char buf[32];
    ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
    if (bytes_read < 0) {
        Log::error("Could not read " + std::string(MOISTURE_SENSOR));
        close(fd);
        return -1;
    }

    buf[bytes_read] = '\0';

    int value = std::stoi(buf); 

    close(fd);
    return value;
};

SystemStatus MoistureSensor::getStatus() {
    return moistureSensorStatus_;
};