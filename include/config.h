#ifndef CONFIG_H
#define CONFIG_H

#define test_str_length 1024
// #define __LF__ 10 // Newline character

#include <chrono>
#include <string>

#include <timestamped.h>

namespace cfg
{

const std::string version = "Motor Control v2019.01.29";
const uint64_t xbee_baud = 38400;
const float loadcell_calibration_factor = -10000;
const std::chrono::seconds ping_period(30);

extern std::chrono::steady_clock::time_point start_time;
extern std::chrono::milliseconds
    loop_period,
    pressure_period,
    temperature_period,
    loadcell_period;

} // namespace cfg

namespace state
{

enum vehicle_phase : uint8_t
{
    pre_oxidizer_loading = 0,
    oxidizer_loading,
    pre_launch_checks,
    launch,
    motor_firing,
    coast_to_apogee
};

extern std::chrono::steady_clock::time_point time;
extern std::chrono::steady_clock::time_point last_ping;
extern int status;
extern vehicle_phase phase;
extern timestamped<float> p1, p2, t1, t2, t3, thrust;
extern unsigned char new_data;
extern unsigned int mode;

std::string str();

// truncates timestamp to milliseconds
template <typename C, typename D>
uint64_t millis(const std::chrono::time_point<C, D> &tp)
{
    return std::chrono::duration_cast<std::chrono
        ::milliseconds>(tp.time_since_epoch()).count();
}

template <typename R, typename P>
uint64_t millis(const std::chrono::duration<R, P> &dur)
{
    return std::chrono::duration_cast<std::chrono
        ::milliseconds>(dur).count();
}

} // namespace state

#endif // DEFAULT_CONFIG_H
