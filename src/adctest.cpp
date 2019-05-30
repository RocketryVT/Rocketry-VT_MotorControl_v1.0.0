
#include <iostream>
#include <vector>
#include <unistd.h>
#include <iomanip>

#include <pin_driver.hpp>

using namespace hardware;

pin_driver driver;

int main()
{
    while (true)
    {
        size_t i = 0;
        for (auto adc : pin_driver::adcs())
        {
            double voltage = driver.read(adc);
            std::cout << i++ << ": " << std::setw(8) << std::left
                << std::fixed << std::setprecision(3)
                << voltage << " ";
        }
        std::cout << "\r" << std::flush;
    }
}
