#ifndef HARDWARE_H
#define HARDWARE_H

#include <fstream>

#include "config.h"

namespace Hardware {

// initializes logging and sensors; returns true if ok
bool init();

// checks if hardware is ok
// returns true on ok
bool ok();

// closes and flushes logs, closes sensor connections
void exit();

// resets all hardware things
void reset();

// write a thing to the log file
void write(unsigned char e);

/**
  Updates data variables by calling functions that control data
  acquisition from connected devices

  INPUT
  unsigned long time - Time of Main Loop iteration

  OUTPUT
  Hexidecimal value, one activated for each datum obtained successfully
         0x00000001 - PRESSURE
         0x00000010 - TEMPERATURE_COMBUSTION
         0x00000100 - Load Cell
*/
void update_data(const std::chrono::steady_clock::time_point& t);

// Closes the stepper motor
void closeStepperMotor();

// Opens the stepper motor
void openStepperMotor();

// reads pressure in oxidizer tank
// @return float = pressure in oxy tank in psi
float get_pressure_1_data();

// reads pressure in combustion chamber in psi
// @return float = pressure in chamber (psi)
float get_pressure_2_data();

// get LED state
bool getLED();

// sets LED state
void setLED(bool state);

}

#endif // HARDWARE_H

