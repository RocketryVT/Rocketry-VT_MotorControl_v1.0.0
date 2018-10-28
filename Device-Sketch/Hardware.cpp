#pragma once

#include "Control.h"
#include "XBee_IO.h"
#include "Hardware.h"
#include "Arduino.h"
#include  "Adafruit_MAX31855.h"

/* LED pin number */
int Hardware::pin_LED = 7;

//NEW CODE
/*sets the analog pin for oxidizer transducer*/
int Hardware::oxidizerTransPin = A1;
/*sets up analog pin for c.chamber transducer*/
int Hardware::combustionTransPin = A2;

/*sets pins for themrocouples*/
int8_t Hardware::preCombThermDOpin = 2;
int8_t Hardware:: preCombThermCSpin = 3;
int8_t Hardware ::preCombThermCLKpin = 4;
int8_t Hardware::combChamberThermDOpin = 5;
int8_t Hardware::combChamberThermCSpin = 6;
int8_t Hardware::combChamberThermCLKpin = 7;

int8_t MAXDO  = 2;
int8_t MAXCS  = 3;
int8_t MAXCLK = 4;

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple2(MAXCLK, MAXCS, MAXDO);

/*intialize both thermocouples*/
Adafruit_MAX31855 preCombThermocouple(Hardware::preCombThermCLKpin, Hardware::preCombThermCSpin, Hardware:: preCombThermDOpin);
Adafruit_MAX31855 combChamberThermocouple(Hardware::combChamberThermCLKpin, Hardware::combChamberThermCSpin, Hardware:: combChamberThermDOpin);

//NEW CODE
/*imports dataFile */
File Hardware::dataFile;

//NEW CODE
//create motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
/*connect stepper with 200 steps per rotation*/
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

/* LED Data */
bool Hardware::is_LED_on = false;

//NEW CODE
/*chip for sd card logger*/
const int Hardware::chipSelect = 4;

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
unsigned char Hardware::update_data() {
  //MODE = 1; //temp
  int valvesOpen = 0; //flag for if valves are open
  if (MODE <= 4 && MODE > 0) {
    PRESSURE_OXIDIZER = readOxidizerPressure(); // Insert Patrick's code here
    PRESSURE_COMBUSTION = 2.f; // Insert Patrick's code here
    TEMPERATURE_PRECOMB = preCombThermocouple.readFarenheit();
    TEMPERATURE_COMBUSTION = combChamberThermocouple.readFarenheit(); // Insert Patrick's code here
    // TEMPERATURE_POSTCOMB = 4; // Insert Patrick's code here, implement ltr
    THRUST = 5.f; // Insert Patrick's code here
    if (MODE == 1) {
      //power to motor
      //Serial.println("MOTOR IS ARMED");
    }

    if (MODE == 2) {
      //Serial.println("MOTOR IS FIRING");
      if (valvesOpen == 0) {
        //open steppr
        //open solenoid
        //ignite ematch
        valvesOpen = 1;
      }
      saveDataToSD();
    }

    if (MODE == 4) {
      //Serial.println("MOTOR STOP COMMANDED");
      //close solenoid
      //close stepper
      dataFile.close();
    }
  }

  return 0x1F; // Insert Patrick's code here, the return value is used for error checking
}

/**
   Creates a file to save data to
*/
void Hardware::initializeSaveFile() {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  dataFile = SD.open("datalog.txt", FILE_WRITE);

  // throws error is file os not opened
  if (!dataFile) {
    Serial.println("error opening datalog.txt");
  }
}

/**
   Saves the data values to a line on a file in the SD card
*/
void Hardware::saveDataToSD() {
  String dataString = "";

  dataString += String(PRESSURE_OXIDIZER);
  dataString += String(PRESSURE_COMBUSTION);
  dataString += String(TEMPERATURE_PRECOMB);
  dataString += String(TEMPERATURE_COMBUSTION);
  dataFile.println(dataString);
}

/**
   Initializes the stepper motor
*/
void Hardware::initializeStepperMotor() {
  AFMS.begin(); //create default freq of 1.6khz
  myMotor->setSpeed(100);  // set to 100 rpm
}

/**
   Opens the stepper motor
*/
void Hardware::openStepperMotor() {
  myMotor->step(560, FORWARD, SINGLE); // number of steps to fully open valve
}
/**
   Closes the stepper motor
*/
void Hardware::closeStepperMotor() {
  myMotor->step(560, BACKWARD, SINGLE); //close valve
}

/**
   Turns on the LED

   INPUT
   bool output -> true to output "LED ON", true by default

   RETURN
   void
*/
void Hardware::turn_LED_on(bool output) {
  if (output) XBee.print("LED ON\n");
  is_LED_on = true;

  // TOGGLE LED ON
  digitalWrite(pin_LED, HIGH);
}

/**
   Turns off the LED

   INPUT
   bool output -> true to output "LED OFF", true by default

   RETURN
   void
*/
void Hardware::turn_LED_off(bool output) {
  if (output) XBee.print("LED OFF\n");
  is_LED_on = false;

  // TOGGLE LED ON
  digitalWrite(pin_LED, LOW);
}

//NEW CODE
/**
   reads pressure in oxidizer tank
   @return float = pressure in psi
*/
float Hardware::readOxidizerPressure()
{
  int sensorVal = analogRead(oxidizerTransPin);
  float voltage = (sensorVal * 5.0) / 1024.0;
  float pressure_psi = (((250.0f * voltage)) - 125.0f);
  return pressure_psi;

}

//NEW CODE
/**
   reads pressure in cobustion chamber
   @return float = pressure in psi
*/
float Hardware::readCombChamberPressure()
{
  int sensorVal = analogRead(combustionTransPin);
  float voltage = (sensorVal * 5.0) / 1024.0;
  float pressure_psi = (((250.0f * voltage)) - 125.0f);
  return pressure_psi;

}

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
float Hardware::calcSafetyFact(float preCombTemp, float combChambTemp, float postCombTemp)
{
  //Find highest temp of inputs
  //via interpolation of data look up table, pull SigYield of aluminum
  //based on motor dimesnsions and comb press, calc hoop stress
  //consider making motor dimensions an input
  //calc safety fact
  return 0.0f;
}






