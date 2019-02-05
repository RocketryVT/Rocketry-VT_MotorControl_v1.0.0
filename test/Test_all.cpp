#include "Test_all.h"
#include "Assert.h"
#include "Test_Transmission.h"
#include "Test_FloatToBinary.h"

void Test::run_tests(bool)
{
	TestFloatConversions();
	TestTransmission();
	return;
}
