/*! \file */

#ifndef CONFIG_H
#define CONFIG_H

#include <chrono>
#include <string>
#include <array>

#include <timestamped.h>

/// Contains configuration constants.
namespace cfg
{

/// The current controller version.
const std::string version = "Motor Control v2019.05.18";

/// The maximum period of time the controller will
/// continue without a ping from ground control
/// before shutting down.
const auto ping_period = std::chrono::minutes(60);

const double max_nitrous_level = 1;

/// The minimum period to wait between disengaging
/// software locks on the motor and related systems.
const auto motor_lock_cooldown = std::chrono::seconds(1);

/// The time at which controller operation began.
extern std::chrono::steady_clock::time_point start_time;

extern std::chrono::milliseconds
    /// The period between control::loop iterations.
    loop_period,
    /// The minimum period between pressure measurements.
    pressure_period,
    /// The minimum period between temperature measurements.
    temperature_period,
    /// The minimum period between nitrous height measurements.
    nitrous_period,
    /// The minimum period between thrust measurements.
    thrust_period;

} // namespace cfg

/// \brief Manages vehicle state variables.
/// \details Contains all of the globally accessible vehicle state
/// variables, such as the current time, launch phase, and
/// basic sensor measurements.
namespace state
{

/// The current time.
extern std::chrono::steady_clock::time_point time;
/// The time of the receipt of the last ping.
extern std::chrono::steady_clock::time_point last_ping;
/// The vehicle's status, a bitmask.
extern uint8_t status;
extern timestamped<float>
    /// The oxidizer tank pressure, in pascals.
    o2p,
    /// The oxidizer tank temperature, in degrees Celsius.
    o2t,
    /// The combustion chamber pressure, in pascals.
    cp,
    /// The combustion chamber temperature, in degrees Celsius.
    ct,
    /// The height of liquid in the N20 tank, in centimeters.
    nh,
    /// The current thrust, in kilonewtons.
    thrust;

/// Voltage read from the analog-to-digital converter, in volts.
/// Stores voltages in order, from 0 to 7:
/// [AI0, AI1, AI2, AI3, AI4, AI5, AI6, VDD]
extern std::array<float, 8> voltage;

/// Converts the vehicle's current state into a human-readable string.
/// \return A std::string containing the vehicle state.
std::string str();

/// Extracts milliseconds from a std::chrono::time_point.
/// \param tp A chrono::time_point.
/// \return A uint64_t of milliseconds.
template <typename C, typename D>
uint64_t millis(const std::chrono::time_point<C, D> &tp)
{
    return std::chrono::duration_cast<std::chrono
        ::milliseconds>(tp.time_since_epoch()).count();
}

/// Extracts milliseconds from a std::chrono::duration.
/// \param dur A chrono::duration.
/// \return A uint64_t of milliseconds.
template <typename R, typename P>
uint64_t millis(const std::chrono::duration<R, P> &dur)
{
    return std::chrono::duration_cast<std::chrono
        ::milliseconds>(dur).count();
}

} // namespace state

#endif // DEFAULT_CONFIG_H
