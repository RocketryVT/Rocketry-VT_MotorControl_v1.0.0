#include <analog_sensor.h>
#include <iostream>

namespace drivers
{

analog_sensor::analog_sensor(uint8_t ain,
    const std::function<double(double)> &calibration)
    : _adc(ain), _fd(adc::fd_open(ain)), _calibration(calibration)
{
    if (_fd < 0)
        std::cerr << "Error: ADC port \"" << (int) ain
            << "\" is inaccessible" << std::endl;
}

analog_sensor::~analog_sensor()
{
    adc::fd_close(_fd);
}

double analog_sensor::read()
{
    return _calibration(raw());
}

double analog_sensor::raw()
{
    unsigned int voltage = 0;
    unsigned int ground = 0;
    adc::read_value(_adc, voltage);
    adc::read_value(7, ground);
    return voltage*1.8/ground;
}

} // namespace drivers
