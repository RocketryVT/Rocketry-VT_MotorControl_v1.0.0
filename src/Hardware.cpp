#include "Hardware.h"
#include "config.h"
#include "Adafruit_MAX31855.h"
#include "XBee_IO.h"
#include "Transmission.h"
#include "HX711.h"
#include "motor.h"

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

Motor motor;
std::ofstream Hardware::logfile("log.bin");

/* LED Data */
bool Hardware::is_LED_on = false;

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
void Hardware::update_data(const std::chrono::steady_clock::time_point& time)
{
    // New Data
	unsigned char nd = 0;
	cfg::DATA_TIME = time;
	
	// Update Pressure
	if (time - State_Data::last_pressure_time
        > cfg::pressure_period) {
		State_Data::DATA_P1 = 0; // get_pressure_1_data(); // Insert Patrick's code here
		State_Data::DATA_P2 = 0; // get_pressure_2_data(); // Insert Patrick's code here
		State_Data::last_pressure_time = time;
		nd |= 0x01;
	}
	
	// Update Temperature
	if (time - State_Data::last_temperature_time
        > cfg::temperature_period) {

		// XBeeIO::XBee.end();
		// logfile.end();
		State_Data::DATA_T1 = 0; // thermocouple_1.readFarenheit();
		State_Data::DATA_T2 = 0; // thermocouple_2.readFarenheit();
		State_Data::DATA_T3 = 0; // thermocouple_3.readFarenheit();
		// XBee.begin(cfg::XBEE_BAUD);
		// logfile.begin(cfg::SD_BAUD);
		
		State_Data::last_temperature_time = time;
		nd |= 0x02;
	}
	
	// Update Load Cell
	if (time - State_Data::last_loadcell_time > cfg::loadcell_period)
    {
		State_Data::DATA_THR = 0; // loadcell.get_units(); // Load cell measure thrust
		State_Data::last_loadcell_time = time;
		nd |= 0x04;
	}
	
	State_Data::NEW_DATA = nd;
	return;
}

// creates a file to save data to
void Hardware::sdcard_openfile()
{
	//logfile.begin(cfg::SD_BAUD); // .doing .begin(baud) will not make a new file
}

// closes currently open save file
void Hardware::sdcard_closefile()
{
	//logfile.end(); // .doing .begin(baud) will not make a new file
}

// saves data values to a line on a file in the SD card
void Hardware::sdcard_write(unsigned int datatype)
{
	auto packet = Transmission::buildPacket(datatype);
    for (auto e : packet)
        XBeeIO::transmit(e);
	// logfile.write((char*) XBeeIO::output_buff, len);
}

// initializes the stepper motor
void Hardware::initializeStepperMotor()
{
    motor.setSpeed(100);  // set to 100 rpm
}

// opens the stepper motor
void Hardware::openStepperMotor()
{
    motor.step(560, FORWARD, SINGLE); // number of steps to fully open valve
}

// closes the stepper motor
void Hardware::closeStepperMotor()
{
    motor.step(560, BACKWARD, SINGLE); //close valve
}

// sets the calibration for the load cell
void Hardware::initializeLoadCell()
{
	//This value is obtained by using the SparkFun_HX711_Calibration sketch
    loadcell.set_scale(cfg::loadcell_calibration_factor);
    //Assuming there is no weight on the scale at start up, reset the scale to 0
	loadcell.tare();
}

// returns oxidizer tank pressure in PSI
float Hardware::get_pressure_1_data()
{
    int sensorVal = 0; // analogRead(cfg::pin_P1);
    float voltage = (sensorVal * 5.0) / 1024.0;
    float pressure_psi = (((250.0f * voltage)) - 125.0f);
    return pressure_psi;
}

// returns combustion chamber pressure in PSI
float Hardware::get_pressure_2_data()
{
    int sensorVal = 0; // analogRead(cfg::pin_P2);
    float voltage = (sensorVal * 5.0) / 1024.0;
    float pressure_psi = (((250.0f * voltage)) - 125.0f);
    return pressure_psi;
}

// turns on LED
void Hardware::turn_LED_on(bool output)
{
    if (output) XBeeIO::transmit("LED ON\n");
    is_LED_on = true;
}

// turns off LED
void Hardware::turn_LED_off(bool output)
{
    if (output) XBeeIO::transmit("LED OFF\n");
    is_LED_on = false;
}


