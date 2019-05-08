#include <iostream>
#include <analog_sensor.h>

int main()
{
    drivers::analog_sensor pressure("voltage0",
        [] (double d) { return d*3 + 12; });

    std::cout << pressure.raw() << std::endl;
    std::cout << pressure.read() << std::endl;

    return 0;
}
