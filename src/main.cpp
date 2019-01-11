#include <iostream>

#include "control_main.h"

int main()
{
    control::setup();

    while (true)
    {
        control::loop();
    }

    return 0;
}
