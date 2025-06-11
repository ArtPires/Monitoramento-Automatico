#ifndef ADS1115_H
#define ADS1115_H

class ADS1115 {
public:
    ADS1115(int i2cAddress = 0x48);
    int readChannel(int channel); // 0 a 3
private:
    int fd_;
    int i2cAddress_;
    void configureChannel(int channel);
};

#endif
