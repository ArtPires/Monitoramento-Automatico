#ifndef _COMMONS_H_
#define _COMMONS_H_

static const char* LOG_FILE_PATH = "/home/pi/logs/log.txt";

enum SystemStatus {
    SATRTING,
    RUNNING,
    STOPPING,
    ERROR
};

enum WaterLevel {
    EMPTY,
    LOWER,
    MEDIUM,
    FULL
};

#endif //_COMMONS_H_