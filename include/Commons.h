#ifndef _COMMONS_H_
#define _COMMONS_H_

#include <cstdint>

#include "Log.h"

#ifdef __aarch64__
#define LOG_FILE_PATH "/home/pi/logs/log.txt"
#else
#define LOG_FILE_PATH "/workspaces/logs/log.txt"
#endif

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

#endif //_COMMONS_H_