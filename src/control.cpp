#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <sstream>

#include <control.h>
#include <logging.h>
#include <Transmission.h>
#include <config.h>
#include <XBee_IO.h>
#include <Hardware.h>

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

    Hardware::init();
    if (!Hardware::ok())
    {
        #ifdef DEBUG
        std::cout << "Hardware init failure" << std::endl;
        #endif
        exit_flag = true;
        fail_flag = true;
        return false;
    }
	
    XBeeIO::init();
    if (!XBeeIO::ok())
    {
        #ifdef DEBUG
        std::cout << "XBee init failure" << std::endl;
        #endif
        exit_flag = true;
        fail_flag = true;
        return false;
    }

    start_time = state::last_ping = 
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

    XBeeIO::update_input_buffer();
    XBeeIO::parse();
    Hardware::loop();

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

    Hardware::exit(code);
    logging::write(Transmission::buildPacket(ss.str()));
    logging::flush();
    logging::exit(code);
}

} // namespace control
