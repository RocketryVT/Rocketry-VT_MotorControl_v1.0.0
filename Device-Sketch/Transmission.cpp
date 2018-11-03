
#include "stdafx.h"
#include "control_main.h"
#include "Arduino.h"
#include "Transmission.h"
#include "Default_Config.h"
#include "Assert.h"
#include "Test_all.h"
#include "XBee_IO.h"

/**
 * Compiles data to a char array so that it can be pushed to the
 * serial port directly. Also this makes it easier to compute a
 * checksum on the data
 *
 * INPUTS
 * char* str -> pointer to char array
 * unsigned int type -> data transmission packet type (see documentation)
 * unsigned int len -> lenght of data packet (see documentation)
 *
 * OUTPUTS
 * void (change this to checksum eventually)
 */
void Transmission::buildPacket(unsigned char* str, unsigned int* len, unsigned int type) {

	using namespace State_Data;
	using namespace Default_Config;
	
	// Header
	str[0] = 0xAA;
	str[1] = 0x14;

	// Chars used to store float bytes
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;
	unsigned int i = 0;
	
	switch (type) {
	case 0x00: // Initialization message
		
		str[3] = 0x00; // Transmit Type
		
		// Fill str with firmware version string
		i = 0;
		while (__PROGRAM_VERSION__[i]){
			str[i] = __PROGRAM_VERSION__[i];
			i++;
		}
		str[i] = __LF__; // '\n'
		i++;
		*len = i;
		return;
	case 0x10:
		str[2] = 2; // Length
		str[3] = 0x10; // Type
		str[4] = MODE; // Mode
		*len = 5;
		return;
	case 0x51: // Cold flow test data
		TIME = micros() - START_TIME;

		// Length
		str[2] = 31;

		// Type
		str[3] = 0x51;

		// Time
		str[4] = (unsigned char)(TIME >> 24) & 0xFF;
		str[5] = (unsigned char)(TIME >> 16) & 0xFF;
		str[6] = (unsigned char)(TIME >> 8) & 0xFF;
		str[7] = (unsigned char)(TIME >> 0) & 0xFF;

		// Mode
		str[8] = MODE;

		// Status
		str[9] = (unsigned char)(STATUS >> 8);
		str[10] = (unsigned char)(STATUS >> 8);

		// Pressure Oxidizer
		floatToChars(PRESSURE_OXIDIZER, &a, &b, &c, &d);
		str[11] = a;
		str[12] = b;
		str[13] = c;
		str[14] = d;

		// Pressure Combustion
		floatToChars(PRESSURE_COMBUSTION, &a, &b, &c, &d);
		str[15] = a;
		str[16] = b;
		str[17] = c;
		str[18] = d;

		// Temperature Oxidizer
		floatToChars(TEMPERATURE_PRECOMB, &a, &b, &c, &d);
		str[19] = a;
		str[20] = b;
		str[21] = c;
		str[22] = d;

		// Temperature Combustion
		floatToChars(TEMPERATURE_COMBUSTION, &a, &b, &c, &d);
		str[23] = a;
		str[24] = b;
		str[25] = c;
		str[26] = d;

		// Thrust
		floatToChars(THRUST, &a, &b, &c, &d);
		str[27] = a;
		str[28] = b;
		str[29] = c;
		str[30] = d;

		// New Data
		str[31] = NEW_DATA;

		// Checksum
		str[32] = 15;
		str[33] = 16;

		// Length
		*len = 34;

		return;
	case 0x52: // Cold flow test data
		TIME = micros() - START_TIME;

		// Length
		str[2] = 38;

		// Type
		str[3] = 0x52;

		// Time
		str[4] = (unsigned char)(TIME >> 24) & 0xFF;
		str[5] = (unsigned char)(TIME >> 16) & 0xFF;
		str[6] = (unsigned char)(TIME >> 8) & 0xFF;
		str[7] = (unsigned char)(TIME >> 0) & 0xFF;

		// Mode
		str[8] = MODE;

		// Status
		str[9] = (unsigned char)(STATUS >> 8);
		str[10] = (unsigned char)(STATUS >> 8);

		// Pressure Oxidizer
		floatToChars(PRESSURE_OXIDIZER, &a, &b, &c, &d);
		str[11] = a;
		str[12] = b;
		str[13] = c;
		str[14] = d;

		// Pressure Combustion
		floatToChars(PRESSURE_COMBUSTION, &a, &b, &c, &d);
		str[15] = a;
		str[16] = b;
		str[17] = c;
		str[18] = d;

		// Temperature Oxidizer
		floatToChars(TEMPERATURE_PRECOMB, &a, &b, &c, &d);
		str[19] = a;
		str[20] = b;
		str[21] = c;
		str[22] = d;

		// Temperature Combustion
		floatToChars(TEMPERATURE_COMBUSTION, &a, &b, &c, &d);
		str[23] = a;
		str[24] = b;
		str[25] = c;
		str[26] = d;

		// Temperature Post Combustion
		floatToChars(TEMPERATURE_POSTCOMB, &a, &b, &c, &d);
		str[27] = a;
		str[28] = b;
		str[29] = c;
		str[30] = d;

		// Thrust
		floatToChars(THRUST, &a, &b, &c, &d);
		str[31] = a;
		str[32] = b;
		str[33] = c;
		str[34] = d;

		// New Data
		str[35] = NEW_DATA;

		// Checksum
		str[36] = 15;
		str[37] = 16;

		// Length
		*len = 38;

		return;
	case 0xB0: // Do Unit Tests
		
		// Run Tests
		run_tests(true);
		
		// Compile string
		for (unsigned int i = 0; i < Test::resstriter; i++ ){
			str[i] = Test::resstr[i];
		}
		
		*len = Test::resstriter; // TODO
		
		return;
	default: // Do nothing
		return;
	}
}

/**
 * Converts a series of unsigned char values to a
 * 32 bit floating point value to four bytes. The most
 * significant byte is transmitted first.
 *
 * INPUTS
 * char a -> Most significant byte
 * char b -> 2nd most significant byte
 * char c -> 3rd most significant byte
 * char d -> Least significant byte
 *
 * OUTPUTS
 * float -> Floating point value
 */
float Transmission::charsToFloat(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	unsigned long val = (a << 24) | (b << 16) | (c << 8) | d;
	return *(float*)&val;
}

/**
 * Converts a 32 bit floating point value to four bytes by
 * assigning values to the non-const char pointers. The most
 * significant byte is transmitted first.
 *
 * INPUTS
 * float x -> floating point value
 * char* a -> Most significant byte
 * char* b -> 2nd most significant byte
 * char* c -> 3rd most significant byte
 * char* d -> Least significant byte
 *
 * OUTPUTS
 * N/A
 */
void Transmission::floatToChars(float x, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) {
	unsigned long i = *(unsigned long *)&x;
	*a = (i >> 24) & 0xFF;
	*b = (i >> 16) & 0xFF;
	*c = (i >> 8) & 0xFF;
	*d = i & 0xFF;
}
