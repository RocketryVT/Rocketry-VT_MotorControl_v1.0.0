#include <iostream>
#include <chrono>
#include <thread>

#include "control.h"
#include "Default_Config.h"
#include "XBee_IO.h"
#include "Hardware.h"
#include "Adafruit_MAX31855.h"

void control::init()
{
    std::cout << "CONTROLLER INIT" << std::endl;
	Hardware::initializeStepperMotor();
	Hardware::initializeLoadCell();
	
	Default_Config::START_TIME = Default_Config::DATA_TIME =
        std::chrono::steady_clock::now();;
	
	control::reset();
}

void control::loop()
{
    Default_Config::TIME = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point
        t_lastxbeewrite = Default_Config::TIME,
        t_lastreceivepacket = Default_Config::TIME,
        t_sentpacket = Default_Config::TIME;

    /* Reset Sensor Timings */
    Hardware::update_data(Default_Config::TIME);
    State_Data::LAST_PRESSURE_TIME_US = Default_Config::TIME;
    State_Data::LAST_TEMPERATURE_TIME_US = Default_Config::TIME;
    State_Data::LAST_LOADCELL_TIME_US = Default_Config::TIME;

    int mode_previous = State_Data::MODE;

    // Parse Input buffer and respond to commnands
    if (Default_Config::TIME - t_lastreceivepacket >
        Default_Config::checkbuffer_period_ms)
    {
        XBeeIO::update_input_buffer();
        XBeeIO::parse_input_buffer();
        t_lastreceivepacket = Default_Config::TIME;
        XBeeIO::dispbuff();
    }

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
        Hardware::sdcard_write(Default_Config::DATA_OUT_TYPE);
        if (Default_Config::TIME - t_lastxbeewrite >
            Default_Config::XBeeWrite_period_ms)
        {
            XBeeIO::transmit_data(Default_Config::DATA_OUT_TYPE);
            t_lastxbeewrite = Default_Config::TIME;
        }
    }

    // Iterate
    mode_previous = State_Data::MODE;

    // Time to delay
    // auto after_t = std::chrono::steady_clock::now();
    // auto dt = after_t - Default_Config::TIME;
    // while (dt > Default_Config::LOOP_PERIOD_MS)
    // dt -= Default_Config::LOOP_PERIOD_MS;
    // auto t_wait = Default_Config::LOOP_PERIOD_MS - dt; 
    std::this_thread::sleep_for(Default_Config::LOOP_PERIOD_MS);
}

void control::reset()
{
    std::cout << "CONTROLLER RESET" << std::endl;
	// Shutdown procedure
	
	/* I/O Reset */
	XBeeIO::clear_input_buffer();
	std::this_thread::sleep_for(Default_Config::LOOP_PERIOD_MS);
	// XBeeIO::transmit_data(0x00);
	
	/* Control Data */
	State_Data::MODE = 0;
    Default_Config::START_TIME = std::chrono::steady_clock::now();
	Hardware::sdcard_closefile();
}

int main(int argc, char **argv)
{
    control::init();
    while (1)
    {
        auto fsec = std::chrono::duration_cast<std::chrono::milliseconds>(
            Default_Config::TIME - Default_Config::START_TIME);
        std::cout << std::dec << fsec.count()
            << "          \r" << std::flush;
        control::loop();
    }
    return 0;
}

