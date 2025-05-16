#include <iostream>
#include <wiringPi.h>

int main() {
    wiringPiSetup();
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    std::cout << "Pino 0 configurado como OUTPUT HIGH pela lib WiringPi!" << std::endl;
    return 0;
}
