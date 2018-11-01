#pragma once

#include <SD.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h> //Wire library

namespace Hardware {

/* LED pin number */
extern int pin_LED;

//NEW STUFF----------------------
/* Analog pin to read Voltage from for pressure tranny in oxy tank*/
extern int oxidizerTransPin;
/*analog pin to read off of for comb. chamber pressure*/
extern int combustionTransPin;

/*digital pins for  thermocouples*/
extern int8_t preCombThermDOpin;
extern int8_t preCombThermCSpin;
extern int8_t preCombThermCLKpin;
extern int8_t combChamberThermDOpin;
extern int8_t combChamberThermCSpin;
extern int8_t combChamberThermCLKpin;

//NEW CODE
/*chip select pin for SD logger*/
extern const int chipSelect;

//NEW CODE
/*data file for logging to sd*/
extern File dataFile;

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

/* Stepper Moteror

  /* LED Data */
extern bool is_LED_on;

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
unsigned char update_data();

/**
   Creates a file to save data to
*/
void initializeSaveFile();

/**
   Saves the data values to a line on a file in the SD card
*/
void saveDataToSD();

/**
   Initializes the stepper motor
*/
void initializeStepperMotor();

/**
   Closes the stepper motor
*/
void closeStepperMotor();

/**
   Opens the stepper motor
*/
void openStepperMotor();

/**
  Turns on the LED

  INPUT
  bool output -> true to output "LED ON", true by default

  RETURN
  void
*/
void turn_LED_on(bool output = false);

/**
  Turns off the LED

  INPUT
  bool output -> true to output "LED OFF", true by default

  RETURN
  void
*/
void turn_LED_off(bool output = false);

//NEW CODE
/**reads pressure in oxidizer tank*/
/**
   @return float = pressure in oxy tank in psi
*/
float readOxidizerPressure();


//NEW CODE
/**
   reads pressure in combustion chamber in psi
   @return float = pressure in chamber (psi)
*/
float readCombChamberPressure();

//NEW CODE
/**
   calculates motor safety factor
   at any given time based on temp and
   combustion chamb. pressure.
   Uses highest temp of aluminum, assumes
   motor is 6061 Al. Dimensisons hardcoded for now
   @param preCombTemp = Temp of precombustion chamber
   @param combChambTemp = temp of main combustion chamber
   @param postCombTemp = temperature of post combustion chamber
   @return float = calculated safety falctor
*/
float calcSafetyFact(float preCombTemp, float combChambTemp, float postCombTemp);


}




