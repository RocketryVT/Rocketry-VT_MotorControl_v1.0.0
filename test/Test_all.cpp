#include "Test_all.h"
#include "Assert.h"
#include "Test_Transmission.h"
#include "Test_FloatToBinary.h"

void run_tests(bool istestondevice)
{
	using namespace Test;
	// Initialize test results string
	
	resstriter = 0;
	
	// Do Tests
	TestFloatConversions();
	TestTransmission();
	if (istestondevice) {
		// Run tests on Arduino
		// Ouput Test Results String to serial port as response
	}
	else {
		// Run tests on host computer only
	}
	return;
}
