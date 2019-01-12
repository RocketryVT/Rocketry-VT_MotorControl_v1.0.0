
// #include "control_main.h"
#include "Transmission.h"
#include "Default_Config.h"
#include "Assert.h"
// #include "Test_all.h"
#include "XBee_IO.h"

const unsigned long long integerTime = 0;

/**
* Compiles data to a char array so that it can be pushed to the
* serial port directly. Also this makes it easier to compute a
* checksum on the data
*
* INPUTS
* char* str -> pointer to char array
* unsigned int* len -> lenght of data packet (see documentation)
* unsigned int type -> data transmission packet type (see documentation)
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
	unsigned char c0 = 0;
	unsigned char c1 = 0;
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
	case 0x01: // SD Card Begin of Save File
		str[2] = 6;
		str[3] = 0x01;
		xorchecksum(str, 4, &c0, &c1);
		str[4] = c0; // Checksum
		str[5] = c1; // Checksum
		*len = 6;
		return;
	case 0x02: // SD Card End of Save File
		str[2] = 6;
		str[3] = 0x02;
		xorchecksum(str, 4, &c0, &c1);
		str[4] = c0; // Checksum
		str[5] = c1; // Checksum
		*len = 6;
		return;
	case 0x10:
		str[2] = 7; // Length
		str[3] = 0x10; // Type
		str[4] = MODE; // Mode
		xorchecksum(str, 5, &c0, &c1);
		str[5] = c0; // Checksum
		str[6] = c1; // Checksum
		*len = 7;
		return;
	case 0x40: // Solid Motor Static Fire Tests 2018-11-11
		TIME = DATA_TIME;

		// Length
		*len = 22;
		str[2] = *len;

		// Type
		str[3] = 0x40;

		// Time
		str[4] = (unsigned char)(integerTime >> 24) & 0xFF;
		str[5] = (unsigned char)(integerTime >> 16) & 0xFF;
		str[6] = (unsigned char)(integerTime >> 8) & 0xFF;
		str[7] = (unsigned char)(integerTime >> 0) & 0xFF;

		// Mode
		str[8] = MODE;

		// Status
		str[9] = (unsigned char)(STATUS >> 8);
		str[10] = (unsigned char)(STATUS >> 8);

		// Temperature Oxidizer
		floatToChars(DATA_T1, &a, &b, &c, &d);
		str[11] = a;
		str[12] = b;
		str[13] = c;
		str[14] = d;

		// Thrust
		floatToChars(DATA_THR, &a, &b, &c, &d);
		str[15] = a;
		str[16] = b;
		str[17] = c;
		str[18] = d;

		// New Data
		str[19] = NEW_DATA;

		// Checksum
		xorchecksum(str, *len-2, &c0, &c1);
		str[20] = c0;
		str[21] = c1;

		return;
	case 0x51: // Cold flow test data
		TIME = DATA_TIME;

		// Length
		*len = 34;
		str[2] = *len;
		
		// Type
		str[3] = 0x51;

		// Time
		str[4] = (unsigned char)(integerTime >> 24) & 0xFF;
		str[5] = (unsigned char)(integerTime >> 16) & 0xFF;
		str[6] = (unsigned char)(integerTime >> 8) & 0xFF;
		str[7] = (unsigned char)(integerTime >> 0) & 0xFF;

		// Mode
		str[8] = MODE;

		// Status
		str[9] = (unsigned char)(STATUS >> 8);
		str[10] = (unsigned char)(STATUS >> 8);

		// Pressure Oxidizer
		floatToChars(DATA_P1, &a, &b, &c, &d);
		str[11] = a;
		str[12] = b;
		str[13] = c;
		str[14] = d;

		// Pressure Combustion
		floatToChars(DATA_P2, &a, &b, &c, &d);
		str[15] = a;
		str[16] = b;
		str[17] = c;
		str[18] = d;

		// Temperature Oxidizer
		floatToChars(DATA_T1, &a, &b, &c, &d);
		str[19] = a;
		str[20] = b;
		str[21] = c;
		str[22] = d;

		// Temperature Combustion
		floatToChars(DATA_T2, &a, &b, &c, &d);
		str[23] = a;
		str[24] = b;
		str[25] = c;
		str[26] = d;

		// Thrust
		floatToChars(DATA_THR, &a, &b, &c, &d);
		str[27] = a;
		str[28] = b;
		str[29] = c;
		str[30] = d;

		// New Data
		str[31] = NEW_DATA;

		// Checksum
		xorchecksum(str, *len-2, &c0, &c1);
		str[32] = c0;
		str[33] = c1;

		return;
	case 0x52: // Cold flow test data
		TIME = DATA_TIME;

		// Length
		*len = 38;
		str[2] = *len;

		// Type
		str[3] = 0x52;

		// Time
		str[4] = (unsigned char)(integerTime >> 24) & 0xFF;
		str[5] = (unsigned char)(integerTime >> 16) & 0xFF;
		str[6] = (unsigned char)(integerTime >> 8) & 0xFF;
		str[7] = (unsigned char)(integerTime >> 0) & 0xFF;

		// Mode
		str[8] = MODE;

		// Status
		str[9] = (unsigned char)(STATUS >> 8);
		str[10] = (unsigned char)(STATUS >> 8);

		// Pressure Oxidizer
		floatToChars(DATA_P1, &a, &b, &c, &d);
		str[11] = a;
		str[12] = b;
		str[13] = c;
		str[14] = d;

		// Pressure Combustion
		floatToChars(DATA_P2, &a, &b, &c, &d);
		str[15] = a;
		str[16] = b;
		str[17] = c;
		str[18] = d;

		// Temperature Oxidizer
		floatToChars(DATA_T1, &a, &b, &c, &d);
		str[19] = a;
		str[20] = b;
		str[21] = c;
		str[22] = d;

		// Temperature Combustion
		floatToChars(DATA_T2, &a, &b, &c, &d);
		str[23] = a;
		str[24] = b;
		str[25] = c;
		str[26] = d;

		// Temperature Post Combustion
		floatToChars(DATA_T3, &a, &b, &c, &d);
		str[27] = a;
		str[28] = b;
		str[29] = c;
		str[30] = d;

		// Thrust
		floatToChars(DATA_THR, &a, &b, &c, &d);
		str[31] = a;
		str[32] = b;
		str[33] = c;
		str[34] = d;

		// New Data
		str[35] = NEW_DATA;

		// Checksum
		xorchecksum(str, *len-2, &c0, &c1);
		str[36] = c0;
		str[37] = c1;

		return;
	case 0xB0: // Do Unit Tests
		
		// Run Tests
		// run_tests(true);
		
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
* Computes the exclusive or parity check of the bytes in a message
*
* INPUTS
* char* str -> pointer to char array
* unsigned int len -> lenght of str
* unsigned char* c1 -> First byte of checksum
* unsigned char* c2 -> Second byte of checksum

*/
void Transmission::xorchecksum(unsigned char* str, unsigned int len, unsigned char* c0, unsigned char* c1) {
	*c0 = 0;
	*c1 = 0;
	unsigned int i = 0;
	while (i < len-1) {
		*c0 ^= str[i];
		*c1 ^= str[i + 1];
		i += 2;
	}
	if (i < len) {
		*c0 ^= str[i];
	}
	return;
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
