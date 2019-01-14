#include <iostream>
#include <signal.h>
#include <chrono>

#include "config.h"
#include "control.h"
#include "XBee_IO.h"

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
    }

    std::cout << "Exited successfully." << std::endl;
    return 0;
}

