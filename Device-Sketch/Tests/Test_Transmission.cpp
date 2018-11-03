
#include "stdafx.h"
#include "../Default_Config.h"
#include "../Transmission.h"
#include "Assert.h"
#include "Test_Transmission.h"

int TestTransmission() {
	
	// Initialize
	bool pass = true;
	bool temp = true;
	using namespace std;
	using namespace Test;
	//dmsg("=== Begining Test for Packet Compilation ===");

	// Run tests for each expected output type
	// pass &= assert_true(Test_0x50(), "Failed: 0x50");
	pass &= assert_true(Test_0x10(), "Failed: 0x10");
	pass &= assert_true(Test_0x51(), "Failed: 0x51");
	pass &= assert_true(Test_0x52(), "Failed: 0x52");

	// Ouput results
	if (pass) {
		passed("Test_Transmission");
	}
	else {
		failed("Test_Transmission");
	}
	return 0;
}

/**
* Tests packet 0x51
*/
bool Test_0x51() {
	using namespace std;
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Initialize global data
	START_TIME = 0;
	STATUS = 0;
	PRESSURE_OXIDIZER = 1;
	PRESSURE_COMBUSTION = 2;
	TEMPERATURE_PRECOMB = 3;
	TEMPERATURE_COMBUSTION = 4;
	THRUST = 5;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 170, 20, 31, 81,  0,  1,   226, \
		64,  2,  0,  0,   63, 128, 0,   \
		0,   64, 0,  0,   0,  64,  64,  \
		0,   0,  64, 128, 0,  0,   64,  \
		160, 0,  0,  31,  15, 16 };
	unsigned char out[34];

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x51);

	// Assert cases
	pass &= assert_equals((long)len, (long)34, "Incorrect value on output  len");
	pass &= assert_equals((long)out[0], (long)str[0], "Incorrect value on output  0");
	pass &= assert_equals((long)out[1], (long)str[1], "Incorrect value on output  1");
	pass &= assert_equals((long)out[2], (long)str[2], "Incorrect value on output  2");
	pass &= assert_equals((long)out[3], (long)str[3], "Incorrect value on output  3");
	pass &= assert_equals((long)out[4], (long)str[4], "Incorrect value on output  4");
	pass &= assert_equals((long)out[5], (long)str[5], "Incorrect value on output  5");
	pass &= assert_equals((long)out[6], (long)str[6], "Incorrect value on output  6");
	pass &= assert_equals((long)out[7], (long)str[7], "Incorrect value on output  7");
	pass &= assert_equals((long)out[8], (long)str[8], "Incorrect value on output  8");
	pass &= assert_equals((long)out[9], (long)str[9], "Incorrect value on output  9");
	pass &= assert_equals((long)out[10], (long)str[10], "Incorrect value on output 10");
	pass &= assert_equals((long)out[11], (long)str[11], "Incorrect value on output 11");
	pass &= assert_equals((long)out[12], (long)str[12], "Incorrect value on output 12");
	pass &= assert_equals((long)out[13], (long)str[13], "Incorrect value on output 13");
	pass &= assert_equals((long)out[14], (long)str[14], "Incorrect value on output 14");
	pass &= assert_equals((long)out[15], (long)str[15], "Incorrect value on output 15");
	pass &= assert_equals((long)out[16], (long)str[16], "Incorrect value on output 16");
	pass &= assert_equals((long)out[17], (long)str[17], "Incorrect value on output 17");
	pass &= assert_equals((long)out[18], (long)str[18], "Incorrect value on output 18");
	pass &= assert_equals((long)out[19], (long)str[19], "Incorrect value on output 19");
	pass &= assert_equals((long)out[20], (long)str[20], "Incorrect value on output 20");
	pass &= assert_equals((long)out[21], (long)str[21], "Incorrect value on output 21");
	pass &= assert_equals((long)out[22], (long)str[22], "Incorrect value on output 22");
	pass &= assert_equals((long)out[23], (long)str[23], "Incorrect value on output 23");
	pass &= assert_equals((long)out[24], (long)str[24], "Incorrect value on output 24");
	pass &= assert_equals((long)out[25], (long)str[25], "Incorrect value on output 25");
	pass &= assert_equals((long)out[26], (long)str[26], "Incorrect value on output 26");
	pass &= assert_equals((long)out[27], (long)str[27], "Incorrect value on output 27");
	pass &= assert_equals((long)out[28], (long)str[28], "Incorrect value on output 28");
	pass &= assert_equals((long)out[29], (long)str[29], "Incorrect value on output 29");
	pass &= assert_equals((long)out[30], (long)str[30], "Incorrect value on output 30");
	pass &= assert_equals((long)out[31], (long)str[31], "Incorrect value on output 31");
	pass &= assert_equals((long)out[32], (long)str[32], "Incorrect value on output 32");
	pass &= assert_equals((long)out[33], (long)str[33], "Incorrect value on output 33");

	return pass;
}

/**
* Tests packet 0x10
* This packet outputs the current mode setting of the arduino
*/
bool Test_0x10() {
	using namespace std;
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Initialize global data
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 0xAA, 0x14, 0x02, 0x10, 0x02 };
	unsigned char out[5];

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x10);

	// Assert cases
	pass &= assert_equals((long)len, (long)5, "Incorrect value on output  len");
	pass &= assert_equals((long)out[0], (long)str[0], "Incorrect value on output  0");
	pass &= assert_equals((long)out[1], (long)str[1], "Incorrect value on output  1");
	pass &= assert_equals((long)out[2], (long)str[2], "Incorrect value on output  2");
	pass &= assert_equals((long)out[3], (long)str[3], "Incorrect value on output  3");
	pass &= assert_equals((long)out[4], (long)str[4], "Incorrect value on output  4");

	// Try changing mode
	MODE = 11;
	buildPacket(out, &len, 0x10);
	pass &= assert_equals((long)out[4], 11, "Incorrect value on output  4");

	return pass;
}

/**
* Tests packet 0x52
*/
bool Test_0x52() {
	using namespace std;
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Initialize global data
	START_TIME = 0;
	STATUS = 0;
	PRESSURE_OXIDIZER = 1;
	PRESSURE_COMBUSTION = 2;
	TEMPERATURE_PRECOMB = 3;
	TEMPERATURE_COMBUSTION = 4;
	TEMPERATURE_POSTCOMB = 5;
	THRUST = 6;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 170, 20, 38, 0x52,  0,  1,   226, \
		64,  2,  0,  0,   63,  128, 0,   \
		0,   64, 0,  0,   0,   64,  64,  \
		0,   0,  64, 128, 0,   0,   64,  \
		160, 0,  0,  64,  192, 0,   0,   \
		31,  15, 16 };
	unsigned char out[38];

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x52);

	// Assert cases
	pass &= assert_equals((long)len, (long)38, "Incorrect value on output  len");
	pass &= assert_equals((long)out[0], (long)str[0], "Incorrect value on output  0");
	pass &= assert_equals((long)out[1], (long)str[1], "Incorrect value on output  1");
	pass &= assert_equals((long)out[2], (long)str[2], "Incorrect value on output  2");
	pass &= assert_equals((long)out[3], (long)str[3], "Incorrect value on output  3");
	pass &= assert_equals((long)out[4], (long)str[4], "Incorrect value on output  4");
	pass &= assert_equals((long)out[5], (long)str[5], "Incorrect value on output  5");
	pass &= assert_equals((long)out[6], (long)str[6], "Incorrect value on output  6");
	pass &= assert_equals((long)out[7], (long)str[7], "Incorrect value on output  7");
	pass &= assert_equals((long)out[8], (long)str[8], "Incorrect value on output  8");
	pass &= assert_equals((long)out[9], (long)str[9], "Incorrect value on output  9");
	pass &= assert_equals((long)out[10], (long)str[10], "Incorrect value on output 10");
	pass &= assert_equals((long)out[11], (long)str[11], "Incorrect value on output 11");
	pass &= assert_equals((long)out[12], (long)str[12], "Incorrect value on output 12");
	pass &= assert_equals((long)out[13], (long)str[13], "Incorrect value on output 13");
	pass &= assert_equals((long)out[14], (long)str[14], "Incorrect value on output 14");
	pass &= assert_equals((long)out[15], (long)str[15], "Incorrect value on output 15");
	pass &= assert_equals((long)out[16], (long)str[16], "Incorrect value on output 16");
	pass &= assert_equals((long)out[17], (long)str[17], "Incorrect value on output 17");
	pass &= assert_equals((long)out[18], (long)str[18], "Incorrect value on output 18");
	pass &= assert_equals((long)out[19], (long)str[19], "Incorrect value on output 19");
	pass &= assert_equals((long)out[20], (long)str[20], "Incorrect value on output 20");
	pass &= assert_equals((long)out[21], (long)str[21], "Incorrect value on output 21");
	pass &= assert_equals((long)out[22], (long)str[22], "Incorrect value on output 22");
	pass &= assert_equals((long)out[23], (long)str[23], "Incorrect value on output 23");
	pass &= assert_equals((long)out[24], (long)str[24], "Incorrect value on output 24");
	pass &= assert_equals((long)out[25], (long)str[25], "Incorrect value on output 25");
	pass &= assert_equals((long)out[26], (long)str[26], "Incorrect value on output 26");
	pass &= assert_equals((long)out[27], (long)str[27], "Incorrect value on output 27");
	pass &= assert_equals((long)out[28], (long)str[28], "Incorrect value on output 28");
	pass &= assert_equals((long)out[29], (long)str[29], "Incorrect value on output 29");
	pass &= assert_equals((long)out[30], (long)str[30], "Incorrect value on output 30");
	pass &= assert_equals((long)out[31], (long)str[31], "Incorrect value on output 31");
	pass &= assert_equals((long)out[32], (long)str[32], "Incorrect value on output 32");
	pass &= assert_equals((long)out[33], (long)str[33], "Incorrect value on output 33");
	pass &= assert_equals((long)out[34], (long)str[34], "Incorrect value on output 34");
	pass &= assert_equals((long)out[35], (long)str[35], "Incorrect value on output 35");
	pass &= assert_equals((long)out[36], (long)str[36], "Incorrect value on output 36");
	pass &= assert_equals((long)out[37], (long)str[37], "Incorrect value on output 37");
	pass &= assert_equals((long)out[38], (long)str[38], "Incorrect value on output 38");

	return pass;
}