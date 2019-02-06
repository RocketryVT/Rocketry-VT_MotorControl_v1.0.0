#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <hardware.h>
#include <config.h>

namespace hardware
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
	if (state::o2p.age() > cfg::pressure_period)
		state::o2p = 0;

    if (state::cp.age() > cfg::pressure_period)
        state::cp = 0;

    if (state::o2t.age() > cfg::temperature_period)
        state::o2t = 0;

    if (state::ct.age() > cfg::temperature_period)
        state::ct = 0;

    if (state::nh.age() > cfg::nitrous_period)
        state::nh = 0;

    if (state::thrust.age() > cfg::thrust_period)
        state::thrust = 0;

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
