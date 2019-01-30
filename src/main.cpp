#include <iostream>
#include <signal.h>
#include <chrono>

#include "config.h"
#include "control.h"
#include "XBee_IO.h"

void printLoop()
{
    static size_t state(0);
    state = (state + 1) % 40;

    for (size_t i = 0; i < state; ++i)
        std::cout << ":";
    for (size_t i = 40 - state; i < 40; ++i)
        std::cout << " ";
    std::cout << "\r" << std::flush;
}

int main()
{
    #ifdef DEBUG
    std::cout << "Debug output is enabled" << std::endl;
    #endif

    signal(SIGINT, control::exit);

    if (!control::init())
    {
        std::cout << "Controller init error" << std::endl;
        return 1;
    }
    while (control::ok())
    {
        control::loop();
        printLoop();
    }

    std::cout << "Exited successfully." << std::endl;
    return 0;
}

