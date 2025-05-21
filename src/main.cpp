#include <iostream>
#include <unistd.h>

#include "Log.h"
#include "Commons.h"

#ifdef __aarch64__
#include <wiringPi.h>
void configRpiGPIO() {

    wiringPiSetup();
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    std::cout << "Pino 0 configurado como OUTPUT HIGH pela lib WiringPi!" << std::endl;
}
#endif

void parseArgs(int argc, char* argv[]) {
    int opt;

    if (argc == 1){ return; }

    opt = getopt(argc, argv, "d");
    switch (opt)
    {
    case 'd':
        Log::setLogLevel(LogLevel::DEBUG);
        break;
    default:
        std::string str("Parametro invalido! Utilizar [-d] para habilitar o modo debug!");
        Log::error(str);
        break;
    }
    
}

int main(int argc, char* argv[]) {

    parseArgs(argc, argv);

    Log::info("teste info");
    Log::debug("teste debug");
    Log::warning("teste warning");
    Log::error("teste error");

    return 0;
}
