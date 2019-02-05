#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "Hardware.h"
#include "config.h"
#include "XBee_IO.h"
#include "Transmission.h"

namespace Hardware
{

bool fail_flag = false;

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

/**
   Updates data variables by calling functions that control data
   acquisition from connected devices

   INPUT
   void

   OUTPUT
   Hexidecimal value, one activated for each datum obtained successfully
          0x00000001 - PRESSURE_OXIDIZER
          0x00000010 - PRESSURE_COMBUSTION
          0x00000100 - TEMPERATURE_OXIDIZER
          0x00001000 - TEMPERATURE_COMBUSTION
          0x00010000 - THRUST
*/
void loop()
{
    // New Data
	unsigned char nd = 0;

	// Update Pressure
	if (state::p1.age() > cfg::pressure_period)
    {
		state::p1 = 0; // get_pressure_1_data(); // Insert Patrick's code here
		state::p2 = 0; // get_pressure_2_data(); // Insert Patrick's code here
		nd |= 0b00000001; // 0x01;
	}
	
	// Update Temperature
	if (state::t1.age() > cfg::temperature_period)
    {
		state::t1 = 0; // thermocouple_1.readFarenheit();
		state::t2 = 0; // thermocouple_2.readFarenheit();
		state::t3 = 0; // thermocouple_3.readFarenheit();
		nd |= 0b00000010; // 0x02;
	}
	
	// Update Load Cell
	if (state::thrust.age() > cfg::loadcell_period)
    {
		state::thrust = 0; // loadcell.get_units(); // Load cell measure thrust
		nd |= 0b00000100; // 0x04;
	}
	
	state::new_data = nd;
	return;
}

// returns oxidizer tank pressure in PSI
float get_pressure_1_data()
{
    int sensorVal = 0; // analogRead(cfg::pin_P1);
    float voltage = (sensorVal * 5.0) / 1024.0;
    float pressure_psi = (((250.0f * voltage)) - 125.0f);
    return pressure_psi;
}

// returns combustion chamber pressure in PSI
float get_pressure_2_data()
{
    int sensorVal = 0; // analogRead(cfg::pin_P2);
    float voltage = (sensorVal * 5.0) / 1024.0;
    float pressure_psi = (((250.0f * voltage)) - 125.0f);
    return pressure_psi;
}

// get LED state
bool getLED()
{
    return false;
}

// set LED state
void setLED(bool)
{
}

} // namespace Hardware
