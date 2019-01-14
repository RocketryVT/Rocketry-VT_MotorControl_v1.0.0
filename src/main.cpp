#include <iostream>
#include <signal.h>
#include <chrono>

#include "config.h"
#include "control.h"
#include "XBee_IO.h"

int main()
{
    #ifdef DEBUG
    std::cout << "VERBOSE output is enabled" << std::endl;
    #endif

    signal(SIGINT, control::exit);

    control::init();
    while (control::ok())
    {
        control::loop();
    }

    #ifdef DEBUG
    std::cout << "Exited successfully." << std::endl;
    #endif
    return 0;
}

