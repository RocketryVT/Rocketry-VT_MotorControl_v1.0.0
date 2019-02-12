#include <iostream>
#include <signal.h>
#include <chrono>
#include <cmath>
#include <bitset>

#include <config.h>
#include <control.h>

void printLoop()
{
    uint64_t millis = std::chrono::duration_cast<
        std::chrono::milliseconds>
        (state::time - state::last_ping).count();
    std::cout << std::dec << millis << "  \t";

    std::bitset<8> bits(state::status);
    for (size_t i = 0; i < 8; ++i)
        std::cout << (bits[i] ? "#" : ".");
    std::cout << "      \r" << std::flush;

    /*
    millis = std::chrono::duration_cast<
        std::chrono::milliseconds>
        (state::time - cfg::start_time).count();
    int current = std::cos(millis/1000.0*M_PI + M_PI)*max_bars/2 + max_bars/2;
    for (int i = 0; i <= max_bars; ++i)
    {
        if (i <= current + 2 && i > current - 2) std::cout << ":";
        else std::cout << " ";
    }
    std::cout << "      \r" << std::flush;

    char chars[] = {'/', '-', '\\', '|'};
    static int index = 0;
    index = (index + 1) % 4;
    std::cout << chars[index] << "\r" << std::endl;
    */
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

