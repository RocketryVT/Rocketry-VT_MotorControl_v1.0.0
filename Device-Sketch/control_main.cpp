#include "control_main.h"

//#define XBee Serial1
#include <Adafruit_MotorShield.h>

//#include <SoftwareSerial.h> // XBee library
#include "Default_Config.h"
#include "XBee_IO.h"
#include "Hardware.h"
#include "Adafruit_MAX31855.h"

/**
 * Rocket Motor Control Firmware
 * Outputs raw data and has ability to simulate data by receiving correct command
 */


/************ Function Declarations ************/

/**
 * Setup, runs before anything else
 * 
 * INPUT
 * void
 * 
 * RETURN
 * void
 */
void control::setup() {
	using namespace Default_Config;
	using namespace State_Data;
	
	// Initialize serial ports
	SDCard.begin(SD_BAUD);
	XBee.begin(XBEE_BAUD);
	
	// Initialize hardware
	Hardware::initializeStepperMotor();
	Hardware::initializeLoadCell();
	
	// Call Reset operations
	reset();
}

/**
 * Executes and repeats indefinietly or until the reset command is sent
 * Reset is accomplised by returning from the function and doing an iteration in loop()
 * 
 * INPUT
 * void
 * 
 * RETURN
 * void
 */
void control::loop() {
	using namespace Default_Config;
	using namespace XBeeIO;
	using namespace Hardware;
	using namespace State_Data;
	
	unsigned long t = micros();
	unsigned long t_lastxbeewrite = t;
	unsigned long t_lastsdwrite = t;
	unsigned long t_lastreceivepacket = t;
	unsigned long t_sentpacket = t;
	
	/* Reset Sensor Timings */
	update_data(t);
	State_Data::LAST_PRESSURE_TIME_US = t;
	State_Data::LAST_TEMPERATURE_TIME_US = t;
	State_Data::LAST_LOADCELL_TIME_US = t;
	
	delay(LOOP_PERIOD_MS);
	
	int mode_previous = MODE;
	while (1) {
		t = micros();
		
		// Update State
		update_data(t);
		// TODO: State Control
		
		// Parse Input buffer and respond to commnands
		if (t - t_lastreceivepacket > checkbuffer_period_ms*1000) {
			update_input_buffer();
			parse_input_buffer();
			t_lastreceivepacket = t;
		}
		
		// Mode changes
		if (mode_previous == 0 && MODE != 0) {
			//sdcard_openfile();
			sdcard_write(0x01); // Begin of Test
		}
		else if (mode_previous != 0 && MODE == 0) {
			sdcard_write(0x02); // End of Test
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
		if (MODE == 1 || MODE == 2 || MODE == 3) {
			if (t - t_lastsdwrite > SDWrite_period_ms*1000) {
				sdcard_write(DATA_OUT_TYPE);
				t_lastsdwrite = t;
			}
			if (t - t_lastxbeewrite > XBeeWrite_period_ms*1000) {
				transmit_data(DATA_OUT_TYPE);
				t_lastxbeewrite = t;
			}
		}
		
		// Iterate
		mode_previous = MODE;
		
		// Time to delay
		unsigned long deltat_us = micros() - t;
		unsigned long t_wait = LOOP_PERIOD_MS - ((deltat_us/1000) % LOOP_PERIOD_MS);
		delay( t_wait );
	}
}

void control::reset() {
	using namespace Default_Config;
	
	// Shutdown procedure
	
	/* I/O Reset */
	XBeeIO::clear_input_buffer();
	delay(LOOP_PERIOD_MS);
	XBeeIO::transmit_data(0x00);
	
	/* Control Data */
	State_Data::MODE = 0;
	START_TIME = micros();
	//Hardware::sdcard_closefile();
}

