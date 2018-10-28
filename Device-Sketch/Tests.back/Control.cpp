
#include "stdafx.h"
#include "Control.h"

// Things not included in Hardware.h but that I need access to
unsigned long START_TIME = 0;
unsigned long TIME = 0;
unsigned long micros() {
	return 123456; // Dummy value, same value as what is tested and expected in MATLAB test cases
}