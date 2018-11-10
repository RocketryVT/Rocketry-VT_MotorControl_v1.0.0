
#include "stdafx.h"
#include "Default_Config.h"
#include "Transmission.h"
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
	pass &= assert_true(Test_checksum(), "Failed: checksum");
	pass &= assert_true(Test_0x01(), "Failed: 0x01");
	pass &= assert_true(Test_0x02(), "Failed: 0x02");
	pass &= assert_true(Test_0x10(), "Failed: 0x10");
	pass &= assert_true(Test_0x40(), "Failed: 0x40");
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
 * Tests correct computation of checksum
 */
bool Test_checksum() {

	using namespace Transmission;
	using namespace Test;

	bool pass = true;

	// Initialize
	unsigned char str[5];
	unsigned int len = 5;
	unsigned char c0 = 0;
	unsigned char c1 = 0;
	unsigned char c0_true = 0x00;
	unsigned char c1_true = 0x00;

	// Test 1
	str[0] = 0x01;
	str[1] = 0x02;
	str[2] = 0x01;
	str[3] = 0x02;
	c0 = 0;
	c1 = 0;
	c0_true = 0x00;
	c1_true = 0x00;
	xorchecksum(str, 4, &c0, &c1);
	pass &= assert_equals((long) c0_true, (long) c0, "Test 1 Incorrect c0");
	pass &= assert_equals((long) c1_true, (long) c1, "Test 1 Incorrect c1");


	// Test 2
	str[0] = 0x04;
	str[1] = 0x04;
	str[2] = 0x05;
	str[3] = 0x02;
	c0 = 0;
	c1 = 0;
	c0_true = 0x01;
	c1_true = 0x06;
	xorchecksum(str, 4, &c0, &c1);
	pass &= assert_equals((long)c0_true, (long)c0, "Test 2 Incorrect c0");
	pass &= assert_equals((long)c1_true, (long)c1, "Test 2 Incorrect c1");

	// Test 3
	str[0] = 0x07;
	str[1] = 0x04;
	str[2] = 0x01;
	str[3] = 0x02;
	str[4] = 0x04;
	c0 = 0;
	c1 = 0;
	c0_true = 0x02;
	c1_true = 0x06;
	xorchecksum(str, 5, &c0, &c1);
	pass &= assert_equals((long)c0_true, (long)c0, "Test 3 Incorrect c0");
	pass &= assert_equals((long)c1_true, (long)c1, "Test 3 Incorrect c1");
	
	return true;
}

/**
* Tests packet 0x01
*/
bool Test_0x01() {
	using namespace Test;
	using namespace Transmission;

	bool pass = true;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 0xAA, 0x14, 0x06, 0x01, 0x00, 0x00 };
	unsigned char out[6];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 5, &c0, &c1);
	str[4] = c0;
	str[5] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x01);

	// Assert cases
	pass &= assert_equals((long)len, (long)6, "Incorrect value on output  len");
	for (unsigned int i = 0; i < 6; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "bad str");
	}

	return pass;
}

/**
* Tests packet 0x02
*/
bool Test_0x02() {
	using namespace Test;
	using namespace Transmission;

	bool pass = true;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 0xAA, 0x14, 0x06, 0x02, 0x00, 0x00 };
	unsigned char out[6];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 5, &c0, &c1);
	str[4] = c0;
	str[5] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x02);

	// Assert cases
	pass &= assert_equals((long)len, (long)6, "Incorrect value on output  len");
	for (unsigned int i = 0; i < 6; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "bad str");
	}

	return pass;
}

/**
* Tests packet 0x10
* This packet outputs the current mode setting of the arduino
*/
bool Test_0x10() {
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Save current data
	unsigned long START_TIME_old = START_TIME;
	int           STATUS_old = STATUS;
	float         DATA_P1_old = DATA_P1;
	float         DATA_P2_old = DATA_P2;
	float         DATA_T1_old = DATA_T1;
	float         DATA_T2_old = DATA_T2;
	float         DATA_THR_old = DATA_THR;
	unsigned char NEW_DATA_old = NEW_DATA;
	unsigned int  MODE_old = MODE;

	// Initialize global data
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 0xAA, 0x14, 0x07, 0x10, 0x02, 0x00, 0x00 };
	unsigned char out[7];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 5, &c0, &c1);
	str[5] = c0;
	str[6] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x10);

	// Assert cases
	pass &= assert_equals((long)len, (long)7, "Incorrect value on output  len");
	for (unsigned int i = 0; i < 7; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "Incorrect value on output");
	}

	// Try changing mode
	MODE = 11;
	buildPacket(out, &len, 0x10);
	pass &= assert_equals((long)out[4], 11, "Incorrect value on output  4");

	// Reset Old Data
	START_TIME = START_TIME_old;
	STATUS = STATUS_old;
	DATA_P1 = DATA_P1_old;
	DATA_P2 = DATA_P2_old;
	DATA_T1 = DATA_T1_old;
	DATA_T2 = DATA_T2_old;
	DATA_THR = DATA_THR_old;
	NEW_DATA = NEW_DATA_old;
	MODE = MODE_old;

	return pass;
}

/**
* Tests packet 0x40
*/
bool Test_0x40() {
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Save current data
	unsigned long START_TIME_old = START_TIME;
	int           STATUS_old = STATUS;
	float         DATA_P1_old = DATA_P1;
	float         DATA_P2_old = DATA_P2;
	float         DATA_T1_old = DATA_T1;
	float         DATA_T2_old = DATA_T2;
	float         DATA_THR_old = DATA_THR;
	unsigned char NEW_DATA_old = NEW_DATA;
	unsigned int  MODE_old = MODE;

	// Initialize global data
	START_TIME = 0;
	STATUS = 0;
	DATA_P1 = 1;
	DATA_P2 = 2;
	DATA_T1 = 3;
	DATA_T2 = 4;
	DATA_THR = 5;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 170, 20, 22, \
		0x40,            \
		0,   1, 226, 64, \
		2,               \
		0,  0,           \
		64, 64,  0, 0,   \
		64, 160, 0, 0,   \
		31,              \
		0x00, 0x00 };
	unsigned char out[22];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 20, &c0, &c1);
	str[20] = c0;
	str[21] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x40);

	// Assert cases
	pass &= assert_equals((long)len, (long)22, "Incorrect value on output len");
	for (unsigned int i = 0; i < 22; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "bad str");
	}

	// Reset Old Data
	START_TIME = START_TIME_old;
	STATUS = STATUS_old;
	DATA_P1 = DATA_P1_old;
	DATA_P2 = DATA_P2_old;
	DATA_T1 = DATA_T1_old;
	DATA_T2 = DATA_T2_old;
	DATA_THR = DATA_THR_old;
	NEW_DATA = NEW_DATA_old;
	MODE = MODE_old;

	return pass;
}

/**
* Tests packet 0x51
*/
bool Test_0x51() {
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Save current data
	unsigned long START_TIME_old  = START_TIME;
	int           STATUS_old      = STATUS;
	float         DATA_P1_old     = DATA_P1;
	float         DATA_P2_old     = DATA_P2;
	float         DATA_T1_old     = DATA_T1;
	float         DATA_T2_old     = DATA_T2;
	float         DATA_THR_old    = DATA_THR;
	unsigned char NEW_DATA_old    = NEW_DATA;
	unsigned int  MODE_old        = MODE;
	
	// Initialize global data
	START_TIME = 0;
	STATUS = 0;
	DATA_P1 = 1;
	DATA_P2 = 2;
	DATA_T1 = 3;
	DATA_T2 = 4;
	DATA_THR = 5;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 170, 20, 34, \
		0x51,            \
		0,   1, 226, 64, \
		2,               \
		0,  0,           \
		63, 128, 0, 0,   \
		64, 0,   0, 0,   \
		64, 64,  0, 0,   \
		64, 128, 0, 0,   \
		64, 160, 0, 0,   \
		31,              \
		0x00, 0x00 };
	unsigned char out[34];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 32, &c0, &c1);
	str[32] = c0;
	str[33] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x51);
	
	// Assert cases
	for (unsigned int i = 0; i < 34; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "bad str");
	}
	
	// Reset Old Data
	START_TIME = START_TIME_old;
	STATUS = STATUS_old;
	DATA_P1 = DATA_P1_old;
	DATA_P2 = DATA_P2_old;
	DATA_T1 = DATA_T1_old;
	DATA_T2 = DATA_T2_old;
	DATA_THR = DATA_THR_old;
	NEW_DATA = NEW_DATA_old;
	MODE = MODE_old;
	
	return pass;
}

/**
* Tests packet 0x52
*/
bool Test_0x52() {
	using namespace Test;
	using namespace Transmission;
	using namespace State_Data;
	using namespace Default_Config;

	bool pass = true;

	// Save current data
	unsigned long START_TIME_old   = START_TIME;
	int           STATUS_old       = STATUS;
	float         DATA_P1_old      = DATA_P1;
	float         DATA_P2_old      = DATA_P2;
	float         DATA_T1_old      = DATA_T1;
	float         DATA_T2_old      = DATA_T2;
	float         DATA_T3_old      = DATA_T3;
	float         DATA_THR_old     = DATA_THR;
	unsigned char NEW_DATA_old     = NEW_DATA;
	unsigned int  MODE_old         = MODE;
	
	// Initialize global data
	START_TIME = 0;
	STATUS = 0;
	DATA_P1 = 1;
	DATA_P2 = 2;
	DATA_T1 = 3;
	DATA_T2 = 4;
	DATA_T3 = 5;
	DATA_THR = 6;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	unsigned char str[] = { 170, 20, 38, \
		0x52,             \
		0, 1,    226, 64, \
		2,                \
		0,  0,            \
		63, 128, 0,   0,  \
		64, 0,   0,   0,  \
		64, 64,  0,   0,  \
		64, 128, 0,   0,  \
		64, 160, 0,   0,  \
		64, 192, 0,   0,  \
		31,               \
		0xFF, 0xFF };
	unsigned char out[38];

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(str, 36, &c0, &c1);
	str[36] = c0;
	str[37] = c1;

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	buildPacket(out, &len, 0x52);

	// Assert cases
	for (unsigned int i = 0; i < 38; i++) {
		pass &= assert_equals((long)out[i], (long)str[i], "bad str");
	}

	// Reset Old Data
	START_TIME             = START_TIME_old;
	STATUS                 = STATUS_old;
	DATA_P1                = DATA_P1_old;
	DATA_P2                = DATA_P2_old;
	DATA_T1                = DATA_T1_old;
	DATA_T2                = DATA_T2_old;
	DATA_T3                = DATA_T3_old;
	DATA_THR               = DATA_THR_old;
	NEW_DATA               = NEW_DATA_old;
	MODE                   = MODE_old;
	
	return pass;
}