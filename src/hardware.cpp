#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <hardware.h>
#include <config.h>

#include <led.h>
#include <pressure_sensor.h>
#include <thermocouple.h>

namespace hardware
{

bool fail_flag = false;
uint8_t next_lock = 0;
const uint8_t max_lock = 3;

bool init()
{
    fail_flag = false;
    return ok();
}

// checks if hardware is all good
bool ok()
{
    return !fail_flag;
}

void exit(int)
{
}

void reset()
{

}

void loop()
{
	if (state::o2p.age() > cfg::pressure_period)
		state::o2p = 0;

    if (state::cp.age() > cfg::pressure_period)
        state::cp = 0;

    if (state::o2t.age() > cfg::temperature_period)
        state::o2t = 0;

    if (state::ct.age() > cfg::temperature_period)
        state::ct = 0;

    if (state::nh.age() > cfg::nitrous_period)
        state::nh = 0;

    if (state::thrust.age() > cfg::thrust_period)
        state::thrust = 0;

    drivers::led::set(state::status);

	return;
}

void unlock(uint8_t code)
{
    if (next_lock == max_lock) return;
    else if (code == next_lock) ++next_lock;
    else next_lock = 0;
}

void lock()
{
    next_lock = 0;
}

bool isLocked()
{
    return next_lock < max_lock;
}

uint8_t lockState()
{
    return next_lock;
}

} // namespace Hardware
