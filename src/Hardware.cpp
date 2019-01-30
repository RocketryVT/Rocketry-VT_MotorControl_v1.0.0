#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "Hardware.h"
#include "config.h"
#include "Adafruit_MAX31855.h"
#include "XBee_IO.h"
#include "Transmission.h"
#include "HX711.h"
#include "motor.h"

namespace Hardware
{

/*
// load cell
HX711 loadcell(cfg::LOADCELL_DOUT, cfg::LOADCELL_CLK);

// thermocouples
Adafruit_MAX31855
    thermocouple_1(cfg::pin_T1_CLK,
        cfg::pin_T1_CS, cfg::pin_T1_DO),
    thermocouple_2(cfg::pin_T2_CLK,
        cfg::pin_T2_CS, cfg::pin_T2_DO),
    thermocouple_3(cfg::pin_T3_CLK,
        cfg::pin_T3_CS, cfg::pin_T3_DO);

*/

// Motor motor;
// bool is_LED_on = false;
bool fail_flag = false;

bool init()
{
    fail_flag = false;

    //This value is obtained by using the SparkFun_HX711_Calibration sketch
    // loadcell.set_scale(cfg::loadcell_calibration_factor);
    //Assuming there is no weight on the scale at start up, reset the scale to 0
    // loadcell.tare();

    // init the stepper motor
    // motor.setSpeed(100);  // set to 100 rpm

    return ok();
}

// checks if hardware is all good
bool ok()
{
    return !fail_flag;
}

void exit()
{
}

void reset()
{

}

void write(unsigned char e)
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
void update_data(const std::chrono::steady_clock::time_point& time)
{
    // New Data
	unsigned char nd = 0;
	
	// Update Pressure
	if (time - state::last_pressure_time > cfg::pressure_period)
    {
		state::p1 = 0; // get_pressure_1_data(); // Insert Patrick's code here
		state::p2 = 0; // get_pressure_2_data(); // Insert Patrick's code here
		state::last_pressure_time = time;
		nd |= 0b00000001; // 0x01;
	}
	
	// Update Temperature
	if (time - state::last_temperature_time > cfg::temperature_period)
    {
		state::t1 = 0; // thermocouple_1.readFarenheit();
		state::t2 = 0; // thermocouple_2.readFarenheit();
		state::t3 = 0; // thermocouple_3.readFarenheit();
		
		state::last_temperature_time = time;
		nd |= 0b00000010; // 0x02;
	}
	
	// Update Load Cell
	if (time - state::last_loadcell_time > cfg::loadcell_period)
    {
		state::thrust = 0; // loadcell.get_units(); // Load cell measure thrust
		state::last_loadcell_time = time;
		nd |= 0b00000100; // 0x04;
	}
	
	state::new_data = nd;
	return;
}

// initializes the stepper motor
void initializeStepperMotor()
{
}

// opens the stepper motor
void openStepperMotor()
{
    // motor.step(560, FORWARD, SINGLE); // number of steps to fully open valve
}

// closes the stepper motor
void closeStepperMotor()
{
    // motor.step(560, BACKWARD, SINGLE); //close valve
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
    // return is_LED_on;
}

// set LED state
void setLED(bool state)
{
    // is_LED_on = state;
}

} // namespace Hardware
