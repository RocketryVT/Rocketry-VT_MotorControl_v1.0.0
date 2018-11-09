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
	
	// Baud rates
	Serial.begin(baud);
	XBee.begin(baud);
	
	// Initialize hardware
	Hardware::initializeStepperMotor();
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
	using namespace State_Data;
	
	unsigned long t;
	unsigned long t_sendpacket = micros();
	int mode_previous = MODE;
	while (1) {
		t = micros();
		
		// Update State
		NEW_DATA = Hardware::update_data();
		// TODO: State Control
		
		// Parse Input buffer and respond to commnands
		update_input_buffer();
		parse_input_buffer();
		
		// Mode changes
		if (mode_previous == 0 && MODE > 0) {
			Hardware::sdcard_openfile();
		}
		else if (mode_previous == 3 && MODE != 3) {
			Hardware::sdcard_closefile();
		}
		
		// If it is time to send packet, do so
		
		
		// Wait until next iteration
		
		/* MODE states */
		// 0 - Waiting
		// 1 - Armed
		// 2 - Starting
		// 3 - Firing
		// 4 - Stopped
		// 5 - Simulation
		if (MODE == 1 || MODE == 2 || MODE == 3) {
			// Data Collection
			transmit_data(DATA_OUT_TYPE);
			Hardware::sdcard_write(DATA_OUT_TYPE);
			//saveDataToSD();
		}
		
		// Iterate
		mode_previous = MODE;
		
		// Time to delay
		unsigned long t_delta_us = micros() - t;
		unsigned long t_wait = data_period_ms - ((t_delta_us/1000) % data_period_ms);
		delay( t_wait );
	}
}

void control::reset() {
	using namespace Default_Config;
	XBeeIO::clear_input_buffer();
	delay(data_period_ms);
	XBeeIO::transmit_data(0x00);
	State_Data::MODE = 0;
	START_TIME = micros();
}

