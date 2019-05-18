#ifndef ANALOG_SENSOR
#define ANALOG_SENSOR

#include <functional>
#include <string>
#include <fstream>
#include <gpio.hpp>

namespace drivers
{

class analog_sensor
{
    public:

    analog_sensor(uint8_t ain,
        const std::function<double(double)> &calibration);

    ~analog_sensor();
    double read();

    double raw();

    private:

    int _adc;
    int _fd;
    std::function<double(double)> _calibration;
};

} // namespace drivers

#endif // ANALOG_SENSOR
