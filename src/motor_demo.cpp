#include <chrono>
#include <thread>
#include <iostream>
#include <bitset>

#include <pin_driver.hpp>

int main()
{
    using namespace hardware;
    pin_driver driver;

    for (int i = 0;; ++i)
    {
        switch (i % 4)
        {
            case 0:
                driver.write(gpio_pin::P8_9, false);
                driver.write(gpio_pin::P8_10, false);
                driver.write(gpio_pin::P8_11, false);
                driver.write(gpio_pin::P8_12, false);
                break;
            case 1:
                driver.write(gpio_pin::P8_9, true);
                driver.write(gpio_pin::P8_10, false);
                driver.write(gpio_pin::P8_11, true);
                driver.write(gpio_pin::P8_12, false);
                break;
            case 2:
                driver.write(gpio_pin::P8_9, false);
                driver.write(gpio_pin::P8_10, true);
                driver.write(gpio_pin::P8_11, false);
                driver.write(gpio_pin::P8_12, true);
                break;
            case 3:
                driver.write(gpio_pin::P8_9, true);
                driver.write(gpio_pin::P8_10, true);
                driver.write(gpio_pin::P8_11, true);
                driver.write(gpio_pin::P8_12, true);
                break;
        }

        std::cout << std::bitset<2>(i) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
