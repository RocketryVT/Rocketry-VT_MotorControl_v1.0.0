#ifndef CONFIG_H
#define CONFIG_H

#define test_str_length 1024
// #define __LF__ 10 // Newline character

#include <chrono>
#include <string>

namespace cfg
{
    const std::string version = "Motor Control v2019.01.29";
	const uint64_t xbee_baud = 38400;
    const float loadcell_calibration_factor = -10000;
    const std::chrono::seconds ping_period(20);

    extern std::chrono::steady_clock::time_point start_time;
    extern std::chrono::milliseconds
        /* Device interface control variables */
        loop_period,
        xbee_write_period,
        checkbuffer_period,
        /* Data Timing */
        pressure_period,
        temperature_period,
        loadcell_period;
}

namespace state
{
    extern std::chrono::steady_clock::time_point time;
    extern std::chrono::steady_clock::time_point last_ping;
	extern int status;
	extern float p1;  // Pressure 1
	extern float p2;  // Pressure 2
	extern float t1;  // Temperature 1
	extern float t2;  // Temperature 2
	extern float t3;  // Temperature 3
	extern float thrust; // Thrust / Force
	extern unsigned char new_data;
    extern unsigned int mode;
	
	/* Data Timing Control */
    extern std::chrono::time_point<std::chrono::steady_clock>
	    last_pressure_time,
	    last_temperature_time,
	    last_loadcell_time;
}

#endif // DEFAULT_CONFIG_H
