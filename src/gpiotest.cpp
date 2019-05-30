
#include <iostream>
#include <vector>
#include <unistd.h>
#include <iomanip>

#include <pin_driver.hpp>

using namespace hardware;

int main()
{
    pin_driver driver;

    while (true)
    {
        size_t i = 0;
        for (auto gpio : pin_driver::gpios())
        {
            bool state = driver.read(gpio);
            std::cout << i++ << ": " << (state ? "1" : "0") << " ";
        }
        std::cout << "\r" << std::flush;
    }
}
