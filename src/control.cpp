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
#include <drivers/led.h>

namespace control
{

bool exit_flag = false;
bool fail_flag = false;
std::chrono::steady_clock::time_point start_time;
std::chrono::milliseconds runtime;

bool init()
{
    logging::announce("Controller init", true, true);
    std::stringstream ss;
        ss << "Update rate: "
            << 1000.0/state::millis(cfg::loop_period) << " Hz";
    logging::announce(ss.str(), true, true);

    if (!logging::init())
    {
        logging::announce("Logging init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    hardware::init();
    if (!hardware::ok())
    {
        logging::announce("Hardware init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }
	
    comms::init();
    if (!comms::ok())
    {
        logging::announce("Comms init failure", true, true);
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    start_time = state::time = state::last_ping =
        std::chrono::steady_clock::now();

    return ok();
}

void loop()
{
    state::time = std::chrono::steady_clock::now();
    
    if (state::time - state::last_ping > cfg::ping_period)
    {
        control::exit(5);
    }

    comms::loop();
    hardware::loop();
    logging::loop();

    comms::flush();

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
            case 0: return "Execution completed";
            case 1: return "Soft shutdown";
            case 2: return "SIGINT";
            case 5: return "Ground station connection timeout";
            default: return "[Reserved exit code]";
        }
    };

    exit_flag = true;
    std::stringstream ss;
    ss << "Controller exiting (code " << code << ": " 
        << exitStr(code) << ")";
    logging::announce(ss.str(), true, true);
    hardware::exit(code);
}

} // namespace control
