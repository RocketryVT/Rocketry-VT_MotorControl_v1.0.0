#pragma once

#define test_str_length 1024
#define __PROGRAM_VERSION__ "MOTOR CONTROL 1.0.0"
#define __LF__ 10 // Newline character
#define XBee Serial1
#define SDCard Serial2

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

namespace Default_Config {

	/* Device interface control variables */
	extern unsigned long TIME;
	extern unsigned long data_period_ms;
	extern unsigned long XBeeWrite_period_ms;
	extern unsigned long SDWrite_period_ms;
	extern unsigned long checkbuffer_period_ms;
	extern unsigned long max_time; // What is this?
	extern unsigned long XBEE_BAUD;
	extern unsigned long SD_BAUD;
	extern unsigned int DATA_OUT_TYPE;
	extern unsigned long START_TIME;
}

namespace Pins_Config {
	
	/* Pressure Transducer pins */
	extern unsigned int pin_P1; /* Analog pin to read Voltage from for pressure tranny in oxy tank*/
	extern unsigned int pin_P2; /*analog pin to read off of for comb. chamber pressure*/
	
	/* Thermocouple Pins*/
	extern unsigned int pin_T1_DO;
	extern unsigned int pin_T1_CS;
	extern unsigned int pin_T1_CLK;
	extern unsigned int pin_T2_DO;
	extern unsigned int pin_T2_CS;
	extern unsigned int pin_T2_CLK;
	extern unsigned int pin_T3_DO;
	extern unsigned int pin_T3_CS;
	extern unsigned int pin_T3_CLK;
	
	/* Load Cell Pins */
	extern unsigned int LOADCELL_DOUT;
	extern unsigned int LOADCELL_CLK;
	extern float loadcell_calibration_factor;

	/* LED pin number */
	extern unsigned int pin_LED;
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