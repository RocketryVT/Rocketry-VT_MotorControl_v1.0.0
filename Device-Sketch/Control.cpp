
#include "stdafx.h"
#include "Control.h"


/* Device interface control variables */
unsigned long TIME = 0;
unsigned long data_period_ms = 100;
unsigned long max_time = 30;
unsigned long baud = 38400;
unsigned int DATA_OUT_TYPE = 0x51;
unsigned long START_TIME = 0;

/* Data variables*/
int STATUS = 0;
float PRESSURE_OXIDIZER = 0;
float PRESSURE_COMBUSTION = 0;
float TEMPERATURE_COMBUSTION = 0;
float TEMPERATURE_PRECOMB = 0;
float TEMPERATURE_POSTCOMB = 0; //imp ltr
float THRUST = 0;
unsigned char NEW_DATA = 0;
unsigned int MODE = 0;