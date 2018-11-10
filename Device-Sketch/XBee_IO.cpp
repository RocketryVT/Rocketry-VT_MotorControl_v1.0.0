#include "XBee_IO.h"
#include "Default_Config.h"
#include "control_main.h"
#include "Transmission.h"
#include "Hardware.h"
#include "Arduino.h"
#include "Test_all.h"
#include "Assert.h"

/************ Global Variables ************/
const unsigned char XBeeIO::BUFF_INPUT = 1024;
unsigned char XBeeIO::input_buff[XBeeIO::BUFF_INPUT];
unsigned long XBeeIO::buff_length = 0;
const unsigned char XBeeIO::OUTPUT_BUFF_LENGTH = 255;
unsigned char XBeeIO::output_buff[XBeeIO::OUTPUT_BUFF_LENGTH];

/* Data */
unsigned char XBeeIO::PACKET_SIZE = 30;

/**
 * Recieves input from the XBee and saves it to "input_buff"
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the input_buffer was updated
 */
bool XBeeIO::update_input_buffer() {
	unsigned char char_in;

	if (XBee.available() == 0) return false;
 
	while (XBee.available() > 0) {
		char_in = XBee.read();
		input_buff[buff_length] = char_in;
		buff_length++;
		// TODO: if buff_length is too big, then clear everything
	}
	return true;
}

/**
 * Transmits data over the XBee antenna
 * 
 * INPUT
 * int type -> data packet to transmit
 */
void XBeeIO::transmit_data(unsigned int type) {

	using namespace Transmission;

	/* For debugging only */
	if (type == 0xFF) {
		transmit_data_string();
		return;
	}
	
	// Build packet and transmit it
	unsigned int len = 0;
	buildPacket(output_buff, &len, type);
	XBee.write(output_buff, len);
	return;
}

/** 
 * Parses the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the buffer was parsed succesfully
 */
bool XBeeIO::parse_input_buffer() {

	using namespace Default_Config;
	using namespace State_Data;

	// Empty buffer
	if (buff_length < 4) return false;
	
	// Parse binary command
	// Search for header
	unsigned long i = 0; int is_binary = 0;
	while (i < buff_length) { // TODO: Make this work with trash data before the header
		if (input_buff[i] == 0xAA && input_buff[i+1] == 0x14) { 
			is_binary = true;
			break;
		}
		i++;
	}

	// Decode binary packet
	bool hashdr;
	unsigned long clri = 0;
	if (is_binary) {

		//dispbuff();
	
		while (i < buff_length) {

			hashdr = false;

			// Search for header
			while (i < buff_length)  {
				if (input_buff[i] == 0xAA && input_buff[i+1] == 0x14) { 
					i = i + 3;
					hashdr = true;
					break;
				}
				i = i + 1;
			}
		
			// If incomplete data packet, end loop
			if ( buff_length < 4 && buff_length < (i + input_buff[i-1] - 1)) {
				return false;
			}
	
			// Binary Lexicon
			if (hashdr) {
				if (input_buff[i] == 0x00) { // Echo Firmware Version
					transmit_data(0x00);
				}
				if (input_buff[i] == 0x01) { // Turn LED ON
					Hardware::turn_LED_on();
				}
				else if (input_buff[i] == 0x02) { // Turn LED OFF
					Hardware::turn_LED_off();
				}
				else if (input_buff[i] == 0x03) { // Clear input_buffer
					clear_input_buffer();
				}
				else if (input_buff[i] == 0x04) { // Set MODE
					MODE = input_buff[i+1];
					transmit_data(0x10); // Transmit the current mode
				}
				else if (input_buff[i] == 0x05) { // Unit tests
					transmit_data(0xB0); // Runs the tests
				}
				else if (input_buff[i] == 0x10) {
					// Open Stepper Motor
					Hardware::openStepperMotor();
				}
				else if (input_buff[i] == 0x11) {
					// Close Stepper Motor
					Hardware::closeStepperMotor();
				}
//				else if (input_buff[i] == 0x20) { // Set parameters
//					data_period_ms = input_buff[i+1] + (input_buff[i+2] << 8);
//					max_time = input_buff[i+3]; 
//	
//					if (data_period_ms == 0) data_period_ms = 10;
//				}
//				else if (input_buff[i] == 0x21) { // Display parameters{
//					XBee.print("DATA_PERIOD: ");
//					XBee.print(data_period_ms);
//					XBee.print(" ms\n");
//					XBee.print("MAX_TIME: ");
//					XBee.print(max_time);
//					XBee.print(" s\n");
//				}
//				else if (input_buff[i] == 0x31) { // Set parameters
//					data_period_ms = input_buff[i+1] + (input_buff[i+2] << 8);
//					if (data_period_ms == 0) data_period_ms = 10;
//				}
				else if (input_buff[i] == 0x36) { // Set Output Packet Type
					DATA_OUT_TYPE = input_buff[i+1];
					if (input_buff[i+1] == 0x10) {
						//transmit_data_string();
					}
				}
				else if (input_buff[i] == 0x44) { // Print buffer for debugging
					dispbuff();
				}
				else if (input_buff[i] == 0x50) { // Simple Simulation Packet
					// See documentation for data packet structure
					TIME           = micros();
					DATA_OUT_TYPE  = input_buff[i+0];
					MODE           = input_buff[i+1]; // 5
					STATUS         = input_buff[i+2];
					DATA_P1		   = input_buff[i+3];
					DATA_P2        = input_buff[i+4];
					DATA_T1        = input_buff[i+5];
					DATA_T2        = input_buff[i+6];
					DATA_THR       = input_buff[i+7];
					NEW_DATA       = input_buff[i+8];
				}
				else { // Unrecognized command
					i--;
					continue;
				}
		
				i += input_buff[i-1];
				clri = i;
			}
		}
		
		if (clri) {
			clear_input_buffer(clri);
			return true;
		}
	}

	// Check for soft reset
	i = 3;
	while (i < buff_length) { // TODO: Make this work with trash data before the header
		if (input_buff[i] == 0xFF && input_buff[i-1] == 0xFF && input_buff[i-2] == 0xFF && input_buff[i-3] == 0xFF) { 
			control::reset();
			return true;
		}
		i++;
	}
	
	// Text Lexicon
	if (!strncmp(input_buff, "LED ON", 6)) {
		Hardware::turn_LED_on();
	}
	else if (!strncmp(input_buff, "LED OFF", 7)) {
		Hardware::turn_LED_off();
	}
	else
	{
		return false;
	}

	return true;
}

/** 
 * Clears the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the array was cleared
 */
bool XBeeIO::clear_input_buffer(unsigned long ii) {

	if (ii) {
		for (unsigned long i = ii; i < buff_length; i++) {
			input_buff[i-ii] = input_buff[i];
		}
		buff_length = buff_length - ii;
	}
	else {
		buff_length = 0;
	}
	return true;
}

/**
 * Displays hex values of the buffer to the XBee port
 */
void XBeeIO::dispbuff() {
	XBee.print("BUFFER: ");
	XBee.print(XBeeIO::buff_length);
	XBee.print("\n");
	for (byte i = 0; i < buff_length; i++) {
		XBee.print("	 ");
		XBee.print(input_buff[i], HEX);
		XBee.print("\n");
	}
}

/* Transmits the full data string in ASCII */
void XBeeIO::transmit_data_string() {
		using namespace State_Data;
		XBee.print(micros());
		XBee.print(",");
		XBee.print(STATUS);
		XBee.print(",");
		XBee.print(DATA_P1);
		XBee.print(",");
		XBee.print(DATA_P2);
		XBee.print(",");
		XBee.print(DATA_T1);
		XBee.print(",");
		XBee.print(DATA_T2);
		XBee.print(",");
		XBee.print(DATA_T3);
		XBee.print(",");
		XBee.print(DATA_THR);
		XBee.print(",");
		XBee.print(NEW_DATA);
		XBee.print(",");
		XBee.print(MODE);
		XBee.print(",");
		XBee.print("ENDL");
		XBee.write(__LF__);
}
