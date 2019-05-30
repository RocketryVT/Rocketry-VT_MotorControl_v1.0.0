#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <sstream>

#include <control.h>
#include <logging.h>
#include <transmission.h>
#include <config.h>
#include <comms.h>
#include <hardware.h>
#include <led.h>

namespace control
{

bool exit_flag = false;
bool fail_flag = false;
std::chrono::steady_clock::time_point start_time;
std::chrono::milliseconds runtime;
uint64_t counter = 0;

bool init()
{
    hardware::setLed(255);

    logging::announce("Controller init", true, true);

    if (!logging::init())
    {
        logging::announce("Logging init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    hardware::setLed(0);

    hardware::init();
    if (!hardware::ok())
    {
        logging::announce("Hardware init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    hardware::setLed(255);

    comms::init();
    if (!comms::ok())
    {
        logging::announce("Comms init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    hardware::setLed(0);

    start_time = state::time = state::last_ping =
        std::chrono::steady_clock::now();

    logging::announce("Init complete.", true, true);
    logging::announce(cfg::version(), true, true);

    return ok();
}

void loop()
{
    state::time = std::chrono::steady_clock::now();
    
    if (state::time - state::last_ping > cfg::ping_period)
    {
        control::exit(connection_timeout);
    }

    comms::loop();
    hardware::loop();
    hardware::setLed(state::millis(state::time)/200);
    logging::loop();

    auto next = start_time + runtime + cfg::loop_period;
    runtime += cfg::loop_period;
    std::this_thread::sleep_until(next);
}

void reset()
{
    logging::announce("Controller reset", true, true);

    exit_flag = false;
    fail_flag = false;

    comms::reset();
    hardware::reset();
    logging::reset();
}

bool ok()
{
    return !exit_flag;
}

void exit(int code)
{
    auto exitStr = [] (int code)
    {
        switch (code)
        {
            case execution_complete:
                return "Execution completed";
            case soft_shutdown:
                return "Soft shutdown";
            case sigint:
                return "SIGINT";
            case connection_timeout:
                return "Ground station connection timeout";
            case failed_channel_map:
                return "Failed to map channel to id";
            default: return "[Reserved exit code]";
        }
    };

    exit_flag = true;
    std::stringstream ss;
    ss << "Controller exiting (code " << code << ": " 
        << exitStr(code) << ")";
    logging::announce(ss.str(), true, true);
    hardware::exit(code);
    comms::exit(code);
}

} // namespace control
