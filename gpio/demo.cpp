#include <gpio.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <bitset>

const uint8_t forward_pin = 66, backward_pin = 67;

void open()
{
    gpio::export_gpio(forward_pin);
    gpio::export_gpio(backward_pin);
    gpio::set_dir(forward_pin, true);
    gpio::set_dir(backward_pin, true);
	gpio::set_value(forward_pin, true);
	gpio::set_value(backward_pin, false);
}

void close()
{
    gpio::export_gpio(forward_pin);
    gpio::export_gpio(backward_pin);
    gpio::set_dir(forward_pin, true);
    gpio::set_dir(backward_pin, true);
	gpio::set_value(backward_pin, true);
	gpio::set_value(forward_pin, false);
}

void off()
{
    gpio::export_gpio(forward_pin);
    gpio::export_gpio(backward_pin);
    gpio::set_dir(forward_pin, true);
    gpio::set_dir(backward_pin, true);
	gpio::set_value(backward_pin, false);
	gpio::set_value(forward_pin, false);
}

int main(int argc, char **argv)
{
	/*
	auto now = std::chrono::steady_clock::now();
	auto dt = std::chrono::steady_clock::now() - now;
    uint64_t micros = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
	std::cout << "took " << micros << " micros" << std::endl;
	*/

    for (int i = 0;; i++)
    {
        std::bitset<4> bits(i);
       
        std::cout << bits << std::endl;
 
        for (int j = 0; j < 4; j++)
        {
            bool state = bits[j] == 1;
            gpio::export_gpio(66 + j);
            gpio::set_dir(66 + j, true);
            gpio::set_value(66 + j, state);
        }    

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    /*
    if (argc == 1)
        return 1;
    if (argc == 2)
        open();
    if (argc == 3)
        close();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    off();

    */
    return 0;
}
