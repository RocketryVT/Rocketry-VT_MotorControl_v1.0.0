#include <iostream>
#include <signal.h>
#include <chrono>
#include <cmath>

#include "config.h"
#include "control.h"
#include "XBee_IO.h"

void printLoop()
{
    static const uint8_t max_bars = 40;

    uint64_t millis = std::chrono::duration_cast<
        std::chrono::milliseconds>
        (state::time - state::last_ping).count();
    std::cout << std::dec << millis << "      \r\t";

    millis = std::chrono::duration_cast<
        std::chrono::milliseconds>
        (state::time - cfg::start_time).count();
    int current = std::cos(millis/1000.0*M_PI + M_PI)*max_bars/2 + max_bars/2;
    for (int i = 0; i <= max_bars; ++i)
    {
        if (i <= current + 2 && i > current - 2)
            std::cout << ":";
        else
            std::cout << " ";
    }
    std::cout << "      \r" << std::flush;
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

