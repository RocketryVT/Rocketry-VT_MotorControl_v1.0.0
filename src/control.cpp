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

namespace control
{

bool exit_flag = false;
bool fail_flag = false;
std::chrono::steady_clock::time_point start_time;
std::chrono::milliseconds runtime;

bool init()
{
    #ifdef DEBUG
    std::cout << "Controller init" << std::endl;
    #endif

    if (!logging::init())
    {
        #ifdef DEBUG
        std::cout << "Logging init failure" << std::endl;
        #endif
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    hardware::init();
    if (!hardware::ok())
    {
        #ifdef DEBUG
        std::cout << "Hardware init failure" << std::endl;
        #endif
        exit_flag = true;
        fail_flag = true;
        return false;
    }
	
    comms::init();
    if (!comms::ok())
    {
        #ifdef DEBUG
        std::cout << "Comms init failure" << std::endl;
        #endif
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
    logging::write(transmission::buildPacket(state::str()));

    auto next = start_time + runtime + cfg::loop_period;
    runtime += cfg::loop_period;
    std::this_thread::sleep_until(next);
}

void reset()
{
    #ifdef DEBUG
    std::cout << "Controller reset" << std::endl;
    #endif

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
    #ifdef DEBUG
    std::cout << ss.str() << std::endl;
    #endif

    hardware::exit(code);
    logging::write(transmission::buildPacket(ss.str()));
    logging::flush();
    logging::exit(code);
}

} // namespace control
