#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

#include "control.h"
#include "config.h"
#include "XBee_IO.h"
#include "Hardware.h"
#include "Adafruit_MAX31855.h"

bool exit_flag = 0;

void control::init()
{
    std::cout << "CONTROLLER INIT" << std::endl;
	Hardware::initializeStepperMotor();
	Hardware::initializeLoadCell();
	
	cfg::START_TIME = cfg::DATA_TIME =
        std::chrono::steady_clock::now();
    XBeeIO::init();
    if (!XBeeIO::ok())
    {
        std::cout << "XBee init failure" << std::endl;
    }
	control::reset();
}

void control::loop()
{
    cfg::TIME = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point
        t_lastxbeewrite = cfg::TIME,
        t_lastreceivepacket = cfg::TIME,
        t_sentpacket = cfg::TIME;

    /* Reset Sensor Timings */
    Hardware::update_data(cfg::TIME);
    State_Data::last_pressure_time = cfg::TIME;
    State_Data::last_temperature_time = cfg::TIME;
    State_Data::last_loadcell_time = cfg::TIME;

    int mode_previous = State_Data::MODE;

    // Parse Input buffer and respond to commnands
    if (cfg::TIME - t_lastreceivepacket >
        cfg::checkbuffer_period)
    {
        XBeeIO::update_input_buffer();
        XBeeIO::parse();
        t_lastreceivepacket = cfg::TIME;
    }

    XBeeIO::transmit_data(cfg::DATA_OUT_TYPE);
    XBeeIO::flush();

    // Mode changes
    if (mode_previous == 0 && State_Data::MODE != 0)
    {
        //sdcard_openfile();
        Hardware::sdcard_write(0x01); // Begin of Test
    }
    else if (mode_previous != 0 && State_Data::MODE == 0)
    {
        Hardware::sdcard_write(0x02); // End of Test
        //sdcard_closefile();
        // Motor Shutdown Sequence
    }

    /* MODE states */
    // 0 - Waiting
    // 1 - Armed
    // 2 - Starting
    // 3 - Firing
    // 4 - Stopped
    // 5 - Simulation
    if (State_Data::MODE == 1 || State_Data::MODE == 2 ||
        State_Data::MODE == 3)
    {
        Hardware::sdcard_write(cfg::DATA_OUT_TYPE);
        if (cfg::TIME - t_lastxbeewrite >
            cfg::xbee_write_period)
        {
            XBeeIO::transmit_data(cfg::DATA_OUT_TYPE);
            t_lastxbeewrite = cfg::TIME;
        }
    }

    // Iterate
    mode_previous = State_Data::MODE;

    // Time to delay
    // auto after_t = std::chrono::steady_clock::now();
    // auto dt = after_t - cfg::TIME;
    // while (dt > cfg::LOOP_period)
    // dt -= cfg::LOOP_period;
    // auto t_wait = cfg::LOOP_period - dt; 
    std::this_thread::sleep_for(cfg::loop_period);
}

void control::reset()
{
    std::cout << "CONTROLLER RESET" << std::endl;
	// Shutdown procedure
	
	/* I/O Reset */
    XBeeIO::transmit_data(-1);
	XBeeIO::reset();
	std::this_thread::sleep_for(cfg::loop_period);
	XBeeIO::transmit_data(0x00);
	
	/* Control Data */
	State_Data::MODE = 0;
    cfg::START_TIME = std::chrono::steady_clock::now();
	Hardware::sdcard_closefile();
}

bool control::ok()
{
    return !exit_flag;
}

void control::exit(int code)
{
    std::cout << "Exiting (code " << code << ")" << std::endl;
    exit_flag = true;
}

