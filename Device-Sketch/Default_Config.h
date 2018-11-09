#pragma once

#define test_str_length 1024
#define __PROGRAM_VERSION__ "MOTOR CONTROL 1.0.0"
#define __LF__ 10 // Newline character

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
	extern float DATA_P1;  // Pressure 1
	extern float DATA_P2;  // Pressure 2
	extern float DATA_T1;  // Temperature 1
	extern float DATA_T2;  // Temperature 2
	extern float DATA_T3;  // Temperature 3
	extern float DATA_THR; // Thrust / Force
	extern unsigned char NEW_DATA;
	extern unsigned int MODE;
}