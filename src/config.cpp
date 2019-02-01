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
        pressure_period = std::chrono::milliseconds(20),
        temperature_period = std::chrono::milliseconds(20),
        loadcell_period = std::chrono::milliseconds(20);
}

namespace state
{

/* Initialize Data variables*/
std::chrono::steady_clock::time_point time, last_ping;
int status = 0;
float p1 = 0;  // Pressure 1
float p2 = 0;  // Pressure 3
float t1 = 0;  // Temperature 1
float t2 = 0;  // Temperature 2
float t3 = 0;  // Temperature 3
float thrust = 0; // Thrust / Force
unsigned char new_data = 0;
unsigned int mode = 0;

std::chrono::steady_clock::time_point start_time;

/* Data Timing Control */
// these initialize to the epoch of the clock being used
std::chrono::time_point<std::chrono::steady_clock>
    last_pressure_time,
    last_temperature_time,
    last_loadcell_time;

}
