// TestFloatToBinary.cpp : Defines the entry point for the console application.
//

#include "Transmission.h"
#include "Assert.h"
#include "Test_FloatToBinary.h"


/**
 * So uh, maybe this will work and maybe it won't. The float may be defined
 * differently on Arduino than on Windows. Using these conversions though, I
 * can bypass this a little to make sure that the values are converted correctly
 * on the serial interface.
 *
 * Update: 18/04/29
 * Arduino, Matlab, and Visual C++ use IEEE 754 standard 32 bit float points,
 * this method works
 */
int TestFloatConversions()
{
	using namespace std;
	using namespace Test;
	using namespace Transmission;
	//dmsg("=== Begining Test for binary to float ===");

	bool pass = true;

	// Test conversion from float to char (positive)
	float x = 12345.6789f;
	std::vector<unsigned char> bytes;
	bytes << x;
    pass &= assert_equals(bytes.size(), 4, "vector length is incorrect");
    pass &= assert_equals((long) bytes[0], (long) 0x46,
        "floatToChars (positive) 1: a is incorrect");
	pass &= assert_equals((long) bytes[1], (long) 0x40,
        "floatToChars (positive) 2: b is incorrect");
	pass &= assert_equals((long) bytes[2], (long) 0xE6,
        "floatToChars (positive) 3: c is incorrect");
	pass &= assert_equals((long) bytes[3], (long) 0xB7,
        "floatToChars (positive) 4: d is incorrect");

	// Test conversion from float to char (negative)
	x = -12345.6789f;
	bytes.clear();
    bytes << x;
    pass &= assert_equals(bytes.size(), 4, "vector length is incorrect");
    pass &= assert_equals((long) bytes[0], (long) 0xC6,
        "floatToChars (negative) 1: a is incorrect");
	pass &= assert_equals((long) bytes[1], (long) 0x40,
        "floatToChars (negative) 2: b is incorrect");
	pass &= assert_equals((long) bytes[2], (long) 0xE6,
        "floatToChars (negative) 3: c is incorrect");
	pass &= assert_equals((long) bytes[3], (long) 0xB7,
        "floatToChars (negative) 4: d is incorrect");

	// Test conversion from chars to float (Youtube example)
	x = -8.21529491902680293569190591134E-34;
    bytes.clear();
    bytes = {0x88, 0x88, 0x80, 0x00};
	float y;
    bytes >> y;
    pass &= assert_equals(x, y, 1e-34,
        "charsToFloat (Youtube example): result is incorrect");

	// Test conversion from chars to float (positive)
	x = 12345.6789f;
    bytes.clear();
    bytes = {0x46, 0x40, 0xE6, 0xB7};
	bytes >> y;
    pass &= assert_equals(x, y, 1e-8,
        "charsToFloat (positive): result is incorrect");

	// Test conversion from chars to float (negative)
	x = -12345.6789f;
    bytes.clear();
    bytes = {0xC6, 0x40, 0xE6, 0xB7};
    bytes >> y;
	pass &= assert_equals(x, y, 1e-8,
        "charsToFloat (negative): result is incorrect");

	x = 1234.1234f;
    bytes.clear();
	bytes << x;
    pass &= assert_equals(bytes.size(), 4,
        "charsToFloat (F&B): bytes.size() (#1) is incorrect");
    bytes >> y;
    pass &= assert_equals(x, y, 1e-8,
        "charsToFloat (F&B): result is incorrect");
    pass &= assert_equals(bytes.size(), 0,
        "charsToFloat (F&B): bytes.size() (#2) is incorrect");

	// Ouput results
	if (pass) {
		passed("Test_FloatToBinary.cpp");
	}
	else {
		failed("Test_FloatToBinary.cpp");
	}

    return 0;
}

