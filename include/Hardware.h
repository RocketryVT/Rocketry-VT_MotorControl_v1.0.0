#ifndef HARDWARE_H
#define HARDWARE_H

#include <fstream>

#include "config.h"

namespace Hardware {

/* LED pin number */
extern int pin_LED;

/* Pressure Transducer pins */
extern int pin_P1; /* Analog pin to read Voltage from for pressure tranny in oxy tank*/
extern int pin_P2; /*analog pin to read off of for comb. chamber pressure*/

/*digital pins for  thermocouples
extern int8_t pin_T1_DO;
extern int8_t pin_T1_CS;
extern int8_t pin_T1_CLK;
extern int8_t pin_T2_DO;
extern int8_t pin_T2_CS;
extern int8_t pin_T2_CLK;
extern int8_t pin_T3_DO;
extern int8_t pin_T3_CS;
extern int8_t pin_T3_CLK;*/

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

// Initializes the stepper motor
void initializeStepperMotor();

// Closes the stepper motor
void closeStepperMotor();

// Opens the stepper motor
void openStepperMotor();

// Sets the calibration for the Load Cell
void initializeLoadCell();

// reads pressure in oxidizer tank
// @return float = pressure in oxy tank in psi
float get_pressure_1_data();

// reads pressure in combustion chamber in psi
// @return float = pressure in chamber (psi)
float get_pressure_2_data();

// Turns on the LED
void turn_LED_on(bool output = false);

// Turns off the LED
void turn_LED_off(bool output = false);

}

#endif // HARDWARE_H

