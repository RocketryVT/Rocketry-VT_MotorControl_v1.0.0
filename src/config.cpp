#include "config.h"

namespace cfg
{
    std::chrono::steady_clock::time_point start_time
        = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point last_ping
        = std::chrono::steady_clock::now();
    std::chrono::milliseconds
        /* Device interface control variables */
        loop_period = std::chrono::milliseconds(10),
        xbee_write_period = std::chrono::milliseconds(100),
        checkbuffer_period = std::chrono::milliseconds(100),
        /* Data Timing */
        pressure_period = std::chrono::milliseconds(500),
        temperature_period = std::chrono::milliseconds(1000),
        loadcell_period = std::chrono::milliseconds(2000);
}

namespace state
{

/* Initialize Data variables*/
std::chrono::steady_clock::time_point time, last_ping;
int status = 0;
timestamped<float> p1 = 0, p2 = 0, t1 = 0, t2 = 0,
    t3 = 0, thrust = 0;
vehicle_phase phase;
unsigned char new_data = 0;
unsigned int mode = 0;

std::chrono::steady_clock::time_point start_time;
}
