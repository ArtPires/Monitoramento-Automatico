#ifndef _COMMONS_H_
#define _COMMONS_H_

#include <cstdint>

#include "Log.h"

#ifdef __aarch64__
#include <wiringPi.h>
#define LOG_FILE_PATH "/home/pi/logs/log.txt"
#else
#define LOG_FILE_PATH "/workspaces/logs/log.txt"

// Stubs para testes em x86
#define	INPUT   0
#define	OUTPUT  1
inline void wiringPiSetup() {}
inline void pinMode(int, int) {}
inline void digitalWrite(int, int) {}
inline int digitalRead(int) { return 0; }
#endif

#define WATER_PUMP 0 // GPIO WiringPi 0 | Raspberry Pi pin 11

// TODO(artur.pires): Change pins to I2C pin (WiringPi 8 / Raspberry Pi pin 3)
#define MOISTURE_SENSOR 2 // GPIO WiringPi 2 | Raspberry Pi pin 13
#define WATER_LEVEL_SENSOR 8 // GPIO WiringPi 2 | Raspberry Pi pin 13

#define ACTIVATE 0
#define DEACTIVATE 1

enum SystemStatus : uint8_t {
    STARTING,
    RUNNING,
    STOPPING,
    ERROR
};

enum WaterLevel : uint8_t{
    EMPTY,
    LOWER,
    MEDIUM,
    FULL
};

static uint8_t systemStatus_ = SystemStatus::STARTING;

inline int wiringpi_to_physical[30] = {
    11, 12, 13, 15, 16, 18, 22, 7, 3, 5,
    24, 26, 19, 21, 23, 8, 10, -1, -1, -1,
    -1, 29, 31, 33, 35, 37, 32, 36, 38, 40
};

inline int get_physical_pin(int wp_pin) {
    if (wp_pin >= 0 && wp_pin < 30)
        return wiringpi_to_physical[wp_pin];
    return -1;
};

inline uint64_t getTimeNow(){
    auto now = std::chrono::system_clock::now();
    return static_cast<uint64_t>(std::chrono::system_clock::to_time_t(now));
};

#endif //_COMMONS_H_