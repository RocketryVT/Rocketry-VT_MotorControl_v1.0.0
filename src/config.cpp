#include "config.h"

#include <sstream>

namespace cfg
{

std::chrono::steady_clock::time_point start_time
    = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point last_ping
    = std::chrono::steady_clock::now();
std::chrono::milliseconds
    loop_period = std::chrono::milliseconds(5),
    pressure_period = std::chrono::milliseconds(500),
    temperature_period = std::chrono::milliseconds(1000),
    nitrous_period = std::chrono::milliseconds(1500),
    thrust_period = std::chrono::milliseconds(2500);

} // namespace cfg

namespace state
{

/* Initialize Data variables*/
std::chrono::steady_clock::time_point time, last_ping;
uint8_t status = 0;
timestamped<float> o2p = 0, o2t = 0, cp = 0, ct = 0, nh = 0, thrust = 0;
unsigned char new_data = 0;
unsigned int mode = 0;

std::string str()
{
    std::stringstream ss;
    ss << millis(time) << ", "
       << millis(last_ping) << ", "
       << o2p << ", "
       << o2t << ", "
       << cp << ", "
       << ct << ", "
       << nh << ", "
       << thrust;

    return ss.str();
}

} // namespace state
