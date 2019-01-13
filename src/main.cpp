#include <iostream>
#include <signal.h>
#include <chrono>

#include "config.h"
#include "control.h"
#include "XBee_IO.h"

int main()
{
    signal(SIGINT, control::exit);

    control::init();
    while (control::ok())
    {
        control::loop();
    }

    std::cout << "Exited successfully." << std::endl;
    return 0;
}

