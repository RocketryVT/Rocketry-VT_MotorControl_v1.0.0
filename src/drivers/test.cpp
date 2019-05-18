#include <iostream>
#include <analog_sensor.h>
#include <iomanip>
int main()
{
    drivers::analog_sensor temp(0,
        [] (double v) { return v*79.0; });
    while (true)
        std::cout << std::fixed << std::left << std::setprecision(3)
            << std::setw(10) << temp.raw() << std::setw(10) << temp.read() << "\r" << std::flush;
    return 0;
}
