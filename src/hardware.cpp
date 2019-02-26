#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#include <hardware.h>
#include <config.h>
#include <timestamped.h>

#include <led.h>
#include <pressure_sensor.h>
#include <thermocouple.h>

namespace hardware
{

bool fail_flag = false;
timestamped<uint8_t> next_lock = 0;
const uint8_t max_lock = 3;

bool init()
{
    fail_flag = false;
    next_lock = 0;
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

bool reset()
{
    exit(0);
    return init();
}

void loop()
{
	if (state::o2p.age() > cfg::pressure_period)
		state::o2p = (rand() & 10000)/100.0;

    if (state::cp.age() > cfg::pressure_period)
        state::cp = (rand() & 10000)/100.0;

    if (state::o2t.age() > cfg::temperature_period)
        state::o2t = (rand() & 10000)/100.0;

    if (state::ct.age() > cfg::temperature_period)
        state::ct = (rand() & 10000)/100.0;

    if (state::nh.age() > cfg::nitrous_period)
        state::nh = (rand() & 10000)/100.0;

    if (state::thrust.age() > cfg::thrust_period)
        state::thrust = (rand() & 10000)/100.0;

    drivers::led::set(state::status);

	return;
}

void unlock(uint8_t code)
{
    if (next_lock == max_lock ||
        next_lock.age() < cfg::motor_lock_cooldown)
        return;
    else if (code == next_lock)
    {
        next_lock.update(next_lock + 1);
    }
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

bool continuity()
{
    return false;
}

void disconnectFeedLine()
{

}

} // namespace Hardware
