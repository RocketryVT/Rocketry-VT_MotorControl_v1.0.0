
#include <analog_sensor.h>
#include <iostream>

namespace drivers
{

analog_sensor::analog_sensor(const std::string &fn,
    const std::function<double(double)> &calibration)
    : _calibration(calibration)
{
    _file = std::ifstream(fn);
    if (!_file)
        std::cerr << "Error: voltage file \"" << fn
            << "\" is inaccessible" << std::endl;
}

double analog_sensor::read()
{
    return _calibration(raw());
}

double analog_sensor::raw()
{
    std::string line;
    std::getline(_file, line);
    _file.seekg(0, _file.beg);
    double ret = 0;
    try
    {
        ret = std::stod(line);
    }
    catch (...) { }
    return ret;
}

} // namespace drivers
