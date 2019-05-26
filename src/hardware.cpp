#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <gpio.hpp>
#include <bitset>

#include <hardware.h>
#include <logging.h>
#include <transmission.h>
#include <comms.h>
#include <config.h>
#include <timestamped.h>

namespace hardware
{

bool fill_ongoing;
bool feed_line_connected;
bool fail_flag;
timestamped<uint8_t> next_lock;
const uint8_t max_lock = 3;

bool init()
{
    logging::announce("Init hardware.", false, true);
    fill_ongoing = false;
    feed_line_connected = true;
    fail_flag = false;
    next_lock = 0;
    return ok();
}

// checks if hardware is all good
bool ok()
{
    return !fail_flag;
}

void exit(int code)
{
    std::stringstream ss;
    ss << "Exit hardware with code " << code;
    logging::announce(ss.str(), false, true);
}

bool reset()
{
    exit(0);
    return init();
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
    {
        state::nh = 0;
    }

    if (state::thrust.age() > cfg::thrust_period)
        state::thrust = 0;

    for (auto &e : state::voltage)
        e = 0;

    if (state::nh > cfg::max_nitrous_level)
    {
        logging::announce("Tank full!", false, true);
        auto packet = transmission::buildPacket(
            "/motor/support/tank-full", {});
        // make damn sure support gets this packet
        comms::transmit(packet);
        comms::flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        comms::transmit(packet);
        comms::flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        comms::transmit(packet);
        comms::flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fill_ongoing = false;
    }
}

void unlock(uint8_t code)
{
    if (next_lock == max_lock)
    {
        logging::announce("Already unlocked.", true, true);
    }

    if (next_lock.age() < cfg::motor_lock_cooldown)
    {
        return;
    }
    else if (code == next_lock)
    {
        logging::announce("Lock advanced.", true, true);
        next_lock.update(next_lock + 1);
    }
    else next_lock = 0;

    if (!isLocked())
    {
        logging::announce("Unlocked.", true, true);
    }
}

void lock()
{
    logging::announce("Locked.", true, true);
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

bool feedLineConnected()
{
    return feed_line_connected;
}

void disconnectFeedLine()
{
    logging::announce("Disconnecting feed line.", true, true);
    feed_line_connected = false;
}

void closeValves()
{
    logging::announce("Closing valves. (TODO)", true, true);
}

void openValves()
{
    if (isLocked())
    {
        logging::announce("Cannot open valves: "
            "hardware locked.", true, true);
        return;
    }
    logging::announce("Opening valves. (TODO)", true, true);
}

void beginFill()
{
    if (isLocked())
    {
        logging::announce("Cannot begin fill: "
            "hardware locked.", true, true);
        return;
    }
    if (!feed_line_connected)
    {
        logging::announce("Cannot begin fill: "
            "feed line disconnected.", true, true);
        return;
    }
    logging::announce("Waiting for tank to be filled.", false, true);
    fill_ongoing = true;
}

void stopFill()
{
    logging::announce("Stopping fill.", true, true);
    fill_ongoing = false;
}

bool isFillOngoing()
{
    return fill_ongoing;
}

void setLed(uint8_t bitmask)
{
    std::bitset<4> bits(bitmask);
    for (uint8_t i = 0; i < 4; ++i)
    {
        gpio::led_set_value(i, bits[i]);
    }
}

} // namespace Hardware
