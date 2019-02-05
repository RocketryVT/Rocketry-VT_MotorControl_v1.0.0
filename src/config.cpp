#include "config.h"

#include <sstream>

namespace cfg
{

std::chrono::steady_clock::time_point start_time
    = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point last_ping
    = std::chrono::steady_clock::now();
std::chrono::milliseconds
    loop_period = std::chrono::milliseconds(50),
    pressure_period = std::chrono::milliseconds(500),
    temperature_period = std::chrono::milliseconds(1000),
    loadcell_period = std::chrono::milliseconds(2000);

} // namespace cfg

namespace state
{

/* Initialize Data variables*/
std::chrono::steady_clock::time_point time, last_ping;
int status = 0;
timestamped<float> p1 = 0, p2 = 0, t1 = 0, t2 = 0, t3 = 0, thrust = 0;
vehicle_phase phase;
unsigned char new_data = 0;
unsigned int mode = 0;

std::chrono::steady_clock::time_point start_time;

std::string str()
{
    std::stringstream ss;
    ss << millis(time) << ", " << millis(last_ping) << ", ";

    switch (phase)
    {
        case 0: ss << "PRE-LOADING"; break;
        case 1: ss << "02 LOADING"; break;
        case 2: ss << "PRE-LAUNCH"; break;
        case 3: ss << "IGNITION"; break;
        case 4: ss << "BURNING"; break;
        default: ss << "INVALID PHASE";
    }
    
    ss << ", " << p1 << ", "
       << p2 << ", "
       << t1 << ", "
       << t2 << ", "
       << t3 << ", "
       << thrust;

    return ss.str();
}

} // namespace state
