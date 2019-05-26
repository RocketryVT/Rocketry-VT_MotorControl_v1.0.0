#include <gpio.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <iomanip>

int main()
{
    while (true)
    {
        unsigned int vdd;
        adc::read_value(7, vdd);
        for (int i = 0; i < 8; ++i)
        {
            unsigned int value;
            adc::read_value(i, value);
            double voltage = value*1.8/vdd;
            std::cout << i << ": " << std::setw(8) << std::left
                << std::fixed << std::setprecision(3)
                << voltage << " ";
        }
        std::cout << "\r" << std::flush;
    }
}
