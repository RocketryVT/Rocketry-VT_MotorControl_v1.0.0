#ifndef ANALOG_SENSOR
#define ANALOG_SENSOR

#include <functional>
#include <string>
#include <fstream>

namespace drivers
{

class analog_sensor
{
    public:

    analog_sensor(const std::string &fn,
        const std::function<double(double)> &calibration);

    double read();

    double raw();

    private:

    std::ifstream _file;
    std::function<double(double)> _calibration;
};

} // namespace drivers

#endif // ANALOG_SENSOR
