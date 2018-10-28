#pragma once

/**
 * Dummy header file for saving global variables
 */

#include "stdafx.h"

namespace Hardware {

	// Data included in Hardware.h
	extern int STATUS;
	extern float PRESSURE_OXIDIZER;
	extern float PRESSURE_COMBUSTION;
	extern float TEMPERATURE_OXIDIZER;
	extern float TEMPERATURE_COMBUSTION;
	extern float TEMPERATURE_POST_COMBUSTION;
	extern float THRUST;
	extern unsigned char NEW_DATA;
	extern unsigned int MODE;
}