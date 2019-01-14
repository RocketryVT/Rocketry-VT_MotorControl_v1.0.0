
#include <iostream>
#include <iomanip>

#include "config.h"
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
	std::vector<unsigned char> packet;
	// unsigned int len = 5;
	unsigned char c0;
	unsigned char c1;
	unsigned char c0_true = 0x00;
	unsigned char c1_true = 0x00;

	// Test 1
	packet.push_back(0x01);
	packet.push_back(0x02);
	packet.push_back(0x01);
	packet.push_back(0x02);
	c0_true = 0x00;
	c1_true = 0x00;
	xorchecksum(packet, c0, c1);
	pass &= assert_equals((long) c0_true, (long) c0, "Test 1 Incorrect c0");
	pass &= assert_equals((long) c1_true, (long) c1, "Test 1 Incorrect c1");

	// Test 2
    packet.clear();
    packet.push_back(0x04);
	packet.push_back(0x04);
	packet.push_back(0x05);
	packet.push_back(0x02);
	c0_true = 0x01;
	c1_true = 0x06;
	xorchecksum(packet, c0, c1);
	pass &= assert_equals((long) c0_true, (long) c0, "Test 2 Incorrect c0");
	pass &= assert_equals((long) c1_true, (long) c1, "Test 2 Incorrect c1");

	// Test 3
    packet.clear();
	packet.push_back(0x07);
    packet.push_back(0x04);
	packet.push_back(0x01);
	packet.push_back(0x02);
	packet.push_back(0x04);
	c0_true = 0x02;
	c1_true = 0x06;
	xorchecksum(packet, c0, c1);
	pass &= assert_equals((long) c0_true, (long) c0, "Test 3 Incorrect c0");
	pass &= assert_equals((long) c1_true, (long) c1, "Test 3 Incorrect c1");
	
	return true;
}

/**
* Tests packet 0x01
*/
bool Test_0x01()
{
	using namespace Test;
	using namespace Transmission;

	bool pass = true;

	// Desired output (Obtained using MATLAB Test cases)
    std::vector<unsigned char> packet { 0xAA, 0x14, 0x06, 0x01 };

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	unsigned int len = 0;
	std::vector<unsigned char> out = buildPacket(0x01);

	// Assert cases
	pass &= assert_equals((long) out.size(), (long) 6, "Incorrect value on output  len");
	for (unsigned int i = 0; i < out.size(); i++) {
		pass &= assert_equals((long) out[i], (long) packet[i], "bad str");
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
	// unsigned char str[] = { 0xAA, 0x14, 0x06, 0x02, 0x00, 0x00 };
    std::vector<unsigned char> packet { 0xAA, 0x14, 0x06, 0x02 };

    // Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	auto out = buildPacket(0x02);

	// Assert cases
	pass &= assert_equals((long) out.size(), (long) 6, "Incorrect value on output  len");
	for (unsigned int i = 0; i < out.size(); i++) {
		pass &= assert_equals((long) out[i], (long) packet[i], "bad str");
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
	using namespace cfg;

	bool pass = true;

	// Save current data
	auto          DATA_TIME_old = DATA_TIME;
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
    std::vector<unsigned char> packet { 0xAA, 0x14, 0x07, 0x10, 0x02 };

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	auto out = buildPacket(0x10);

	// Assert cases
	pass &= assert_equals((long) out.size(), (long) 7, "Incorrect value on output  len");
	for (unsigned int i = 0; i < out.size(); i++) {
		pass &= assert_equals((long) out[i], (long) packet[i], "Incorrect value on output");
	}

	// Try changing mode
	MODE = 11;
	out = buildPacket(0x10);
	pass &= assert_equals((long) out[4], 11, "Incorrect value on output  4");

	// Reset Old Data
	DATA_TIME = DATA_TIME_old;
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
	using namespace cfg;

	bool pass = true;

	// Save current data
	auto          DATA_TIME_old = DATA_TIME;
	int           STATUS_old = STATUS;
	float         DATA_P1_old = DATA_P1;
	float         DATA_P2_old = DATA_P2;
	float         DATA_T1_old = DATA_T1;
	float         DATA_T2_old = DATA_T2;
	float         DATA_THR_old = DATA_THR;
	unsigned char NEW_DATA_old = NEW_DATA;
	unsigned int  MODE_old = MODE;

	// Initialize global data
	DATA_TIME = std::chrono::milliseconds(123456) + START_TIME;
	STATUS = 0;
	DATA_P1 = 1;
	DATA_P2 = 2;
	DATA_T1 = 3;
	DATA_T2 = 4;
	DATA_THR = 5;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
    std::vector<unsigned char> packet { 170, 20, 22,
		0x40, 0x00,  0x01, 0xE2, 0x40,
		2, 0, 0, 64, 64, 0, 0, 64, 160, 0, 0, 31 };

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	auto out = buildPacket(0x40);

	// Assert cases
	pass &= assert_equals((long) out.size(), (long) 22, "Incorrect value on output len");
	for (unsigned int i = 0; i < out.size(); i++) {
		pass &= assert_equals((long) out[i], (long) packet[i], "bad str");
	}

	// Reset Old Data
	DATA_TIME = DATA_TIME_old;
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
	using namespace cfg;

	bool pass = true;

	// Save current data
	auto          DATA_TIME_old   = DATA_TIME;
	int           STATUS_old      = STATUS;
	float         DATA_P1_old     = DATA_P1;
	float         DATA_P2_old     = DATA_P2;
	float         DATA_T1_old     = DATA_T1;
	float         DATA_T2_old     = DATA_T2;
	float         DATA_THR_old    = DATA_THR;
	unsigned char NEW_DATA_old    = NEW_DATA;
	unsigned int  MODE_old        = MODE;
	
	// Initialize global data
	DATA_TIME = std::chrono::milliseconds(123456) + START_TIME;
	STATUS = 0;
	DATA_P1 = 1;
	DATA_P2 = 2;
	DATA_T1 = 3;
	DATA_T2 = 4;
	DATA_THR = 5;
	NEW_DATA = 0x1F;
	MODE = 2;

	// Desired output (Obtained using MATLAB Test cases)
	std::vector<unsigned char> packet = { 170, 20, 34,
        0x51, 0, 1, 226, 64, 2, 0, 0, 63, 128, 0, 0, 64, 0, 0, 0,
		64, 64, 0, 0, 64, 128, 0, 0, 64, 160, 0, 0,	31 };

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	auto out = buildPacket(0x51);
	
	// Assert cases
	for (unsigned int i = 0; i < out.size(); i++)
    {
		pass &= assert_equals((long) out[i], (long) packet[i], "bad str");
	}

	// Reset Old Data
	DATA_TIME = DATA_TIME_old;
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
	using namespace cfg;

	bool pass = true;

	// Save current data
	auto          DATA_TIME_old    = DATA_TIME;
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
	DATA_TIME = std::chrono::milliseconds(123456) + START_TIME;
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
	std::vector<unsigned char> packet = { 170, 20, 38, \
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
		31};

	// Do the true checksum
	unsigned char c0;
	unsigned char c1;
	xorchecksum(packet, c0, c1);
	packet.push_back(c0);
	packet.push_back(c1);

	// Test conversion from float to char (positive)
	auto out = buildPacket(0x52);

	// Assert cases
	for (unsigned int i = 0; i < out.size(); i++) {
		pass &= assert_equals((long) out[i], (long) packet[i], "bad str");
	}

	// Reset Old Data
	DATA_TIME   = DATA_TIME_old;
	STATUS      = STATUS_old;
	DATA_P1     = DATA_P1_old;
	DATA_P2     = DATA_P2_old;
	DATA_T1     = DATA_T1_old;
	DATA_T2     = DATA_T2_old;
	DATA_T3     = DATA_T3_old;
	DATA_THR    = DATA_THR_old;
	NEW_DATA    = NEW_DATA_old;
	MODE        = MODE_old;
	
	return pass;
}
