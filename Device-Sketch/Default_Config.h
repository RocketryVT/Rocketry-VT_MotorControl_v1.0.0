#pragma once

#define test_str_length 1024
#define __PROGRAM_VERSION__ "MOTOR CONTROL 1.0.0"

namespace Default_Config {

	/* Device interface control variables */
	extern unsigned long TIME;
	extern unsigned long data_period_ms;
	extern unsigned long max_time;
	extern unsigned long baud;
	extern unsigned int DATA_OUT_TYPE;
	extern unsigned long START_TIME;

}

namespace State_Data {
	/* Data variables*/
	extern int STATUS;
	extern float PRESSURE_OXIDIZER;
	extern float PRESSURE_COMBUSTION;
	extern float TEMPERATURE_PRECOMB;
	extern float TEMPERATURE_COMBUSTION;
	extern float TEMPERATURE_POSTCOMB; //implement ltr
	extern float THRUST;
	extern unsigned char NEW_DATA;
	extern unsigned int MODE;
}