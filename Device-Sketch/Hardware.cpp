#include "Hardware.h"
#include "Default_Config.h"
#include "XBee_IO.h"
#include "Transmission.h"
#include "Arduino.h"
#include "Adafruit_MAX31855.h"
#include "HX711.h"

/* LED pin number */
int Hardware::pin_LED = 7;

//NEW CODE
/*sets the analog pin for oxidizer transducer*/
int Hardware::pin_P1 = A1;
/*sets up analog pin for c.chamber transducer*/
int Hardware::pin_P2 = A2;


/* Load cell object */
#define calibration_factor -10000.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
LS_DOUT = 11;
LS_CLK = 12;
HX711 loadcell(LS_DOUT, LS_CLK);

/*sets pins for themrocouples*/
int8_t Hardware::pin_T1_DO = 2;
int8_t Hardware::pin_T1_CS = 3;
int8_t Hardware::pin_T1_CLK = 4;
int8_t Hardware::pin_T2_DO = 5;
int8_t Hardware::pin_T2_CS = 6;
int8_t Hardware::pin_T2_CLK = 7;
int8_t Hardware::pin_T3_DO = 8;
int8_t Hardware::pin_T3_CS = 9;
int8_t Hardware::pin_T3_CLK = 10;

int8_t MAXDO  = 2;
int8_t MAXCS  = 3;
int8_t MAXCLK = 4;

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple2(MAXCLK, MAXCS, MAXDO);

/*intialize both thermocouples*/
Adafruit_MAX31855 thermocouple_1(Hardware::pin_T1_CLK, Hardware::pin_T1_CS, Hardware:: pin_T1_DO);
Adafruit_MAX31855 thermocouple_2(Hardware::pin_T2_CLK, Hardware::pin_T2_CS, Hardware:: pin_T2_DO);
Adafruit_MAX31855 thermocouple_3(Hardware::pin_T3_CLK, Hardware::pin_T3_CS, Hardware:: pin_T3_DO);

/*imports sdcard_datafile */
File Hardware::sdcard_datafile;

//create motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
/*connect stepper with 200 steps per rotation*/
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

/* LED Data */
bool Hardware::is_LED_on = false;

/*chip for sd card logger*/
const int Hardware::sdcard_chipSelect = 4;

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
  
  using namespace State_Data;
  using namespace Default_Config;
  
  if (MODE <= 4 && MODE > 0) {
    DATA_P1 = get_pressure_1_data(); // Insert Patrick's code here
    DATA_P2 = get_pressure_2_data(); // Insert Patrick's code here
    DATA_T1 = thermocouple_1.readFarenheit();
    DATA_T2 = thermocouple_2.readFarenheit(); // Insert Patrick's code here
	DATA_T3 = thermocouple_3.readFarenheit(); // Insert Patrick's code here
    DATA_THR = loadcell.get_units(); // Load cell measure thrust
  }

  return 0x3F; // Insert Patrick's code here, the return value is used for error checking
}

/**
   Creates a file to save data to
*/
void Hardware::sdcard_openfile() {

  // see if the card is present and can be initialized:
  if (!SD.begin(sdcard_chipSelect)) {
    return;
  }
  sdcard_datafile = SD.open("data.txt", FILE_WRITE);
}

/**
   Closes currently open save file
*/
void Hardware::sdcard_closefile() {
	sdcard_datafile.close();
}

/**
   Saves the data values to a line on a file in the SD card
*/
void Hardware::sdcard_write(unsigned int datatype) {
	unsigned int len = 0;
	Transmission::buildPacket(XBeeIO::output_buff, &len, datatype);
	sdcard_datafile.write(XBeeIO::output_buff, len);
	XBee.println("Writing SD");
	XBee.write(XBeeIO::output_buff, len);
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
   reads pressure in oxidizer tank
   @return float = pressure in psi
*/
float Hardware::get_pressure_1_data()
{
  int sensorVal = analogRead(pin_P1);
  float voltage = (sensorVal * 5.0) / 1024.0;
  float pressure_psi = (((250.0f * voltage)) - 125.0f);
  return pressure_psi;

}

/**
   reads pressure in cobustion chamber
   @return float = pressure in psi
*/
float Hardware::get_pressure_2_data()
{
  int sensorVal = analogRead(pin_P2);
  float voltage = (sensorVal * 5.0) / 1024.0;
  float pressure_psi = (((250.0f * voltage)) - 125.0f);
  return pressure_psi;

}

/**
   Turns on the LED

   INPUT
   bool output -> true to output "LED ON", true by default
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
*/
void Hardware::turn_LED_off(bool output) {
  if (output) XBee.print("LED OFF\n");
  is_LED_on = false;

  // TOGGLE LED ON
  digitalWrite(pin_LED, LOW);
}


