
#include "stdafx.h"
#include "Default_Config.h"

/* Device interface control variables */
unsigned long Default_Config::TIME = 0;
unsigned long Default_Config::data_period_ms = 100;
unsigned long Default_Config::max_time = 30;
unsigned long Default_Config::baud = 38400;
unsigned int Default_Config::DATA_OUT_TYPE = 0x51;
unsigned long Default_Config::START_TIME = 0;

/* Data variables*/
int State_Data::STATUS = 0;
float State_Data::PRESSURE_OXIDIZER = 0;
float State_Data::PRESSURE_COMBUSTION = 0;
float State_Data::TEMPERATURE_COMBUSTION = 0;
float State_Data::TEMPERATURE_PRECOMB = 0;
float State_Data::TEMPERATURE_POSTCOMB = 0; //imp ltr
float State_Data::THRUST = 0;
unsigned char State_Data::NEW_DATA = 0;
unsigned int State_Data::MODE = 0;