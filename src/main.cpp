#include <iostream>
#include <unistd.h>

#include "Log.h"
#include "Commons.h"

#ifdef __aarch64__
#include <wiringPi.h>
void configGPIO() {
    wiringPiSetup();
    pinMode(0, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_DOWN);  // Usa resistor pull-down interno
    Log::info("Pino 11 configurado como OUTPUT");
    Log::info("Pino 13 configurado como INPUT");
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

    configGPIO();

    while (true)
    {
        Log::info(digitalRead(2));

        if (digitalRead(2)){
            digitalWrite(0, LOW);
        } else {
            digitalWrite(0, HIGH);
        }
        usleep(5000);
    }
    

    return 0;
}
