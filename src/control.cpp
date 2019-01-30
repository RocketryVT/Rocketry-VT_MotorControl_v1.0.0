#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

#include <control.h>
#include <logging.h>
#include <config.h>
#include <XBee_IO.h>
#include <Hardware.h>
#include <Adafruit_MAX31855.h>

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

    start_time = std::chrono::steady_clock::now();

    return ok();
}

void loop()
{
    state::time = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point
        t_lastxbeewrite = state::time,
        t_lastreceivepacket = state::time,
        t_sentpacket = state::time;

    // Parse Input buffer and respond to commnands
    if (state::time - t_lastreceivepacket >
        cfg::checkbuffer_period)
    {
        XBeeIO::update_input_buffer();
        XBeeIO::parse();
        t_lastreceivepacket = state::time;
    }

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
    #ifdef DEBUG
    std::cout << "Exiting (code " << code << ")" << std::endl;
    #endif
    exit_flag = true;

    Hardware::exit();
}

} // namespace control
