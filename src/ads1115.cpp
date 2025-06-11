#include "ads1115.h"
#include <wiringPiI2C.h>
#include <unistd.h>

ADS1115::ADS1115(int i2cAddress) : i2cAddress_(i2cAddress) {
    fd_ = wiringPiI2CSetup(i2cAddress_);
}

void ADS1115::configureChannel(int channel) {
    int config = 0x4000; // modo de canal único AINx vs GND

    switch (channel) {
        case 0: config |= 0x4000; break; // AIN0
        case 1: config |= 0x5000; break; // AIN1
        case 2: config |= 0x6000; break; // AIN2
        case 3: config |= 0x7000; break; // AIN3
    }

    config |= 0x8000; // start single conversion
    config |= 0x0003; // 860 SPS
    config |= 0x0100; // single-shot mode
    config |= 0x0000; // FS = ±6.144V

    wiringPiI2CWriteReg16(fd_, 0x01, (config >> 8) | ((config & 0xFF) << 8));
}

int ADS1115::readChannel(int channel) {
    configureChannel(channel);
    usleep(10000); // aguarda conversão (~8ms suficiente)

    int result = wiringPiI2CReadReg16(fd_, 0x00);
    // inverter bytes (big-endian vs little-endian)
    result = ((result & 0xFF) << 8) | ((result >> 8) & 0xFF);

    return result;
}
