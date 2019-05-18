#include <gpio.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include <filters.hpp>
#include <chrono>

int main()
{
    std::vector<int> adcs;
    std::vector<low_pass> lpfs;   

    for (int i = 0; i < 8; ++i)
    {
        adcs.push_back(adc::fd_open(i));
        lpfs.push_back(low_pass(0.15));
    }

    using namespace std::chrono;
    steady_clock::time_point now = steady_clock::now();

    unsigned int value;
    double dt = 0;
    while (true)
    {
        now = steady_clock::now();
        unsigned int vdd;
        adc::read_value(7, vdd);
        for (int i = 0; i < 8; ++i)
        {
            adc::read_value(i, value);
            double voltage = value*1.8/vdd;
            std::cout << i << ": " << std::setw(8) << std::left
                << std::fixed << std::setprecision(3)
                << lpfs[i].step(voltage, dt) << " ";
        }
        std::cout << "\r" << std::flush;
        dt = duration<double>(steady_clock::now() - now).count();
    }
    
    for (auto a : adcs)
        adc::fd_close(a);
}
