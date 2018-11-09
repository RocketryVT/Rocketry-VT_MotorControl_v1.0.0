
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
float State_Data::DATA_P1 = 0;  // Pressure 1
float State_Data::DATA_P2 = 0;  // Pressure 3
float State_Data::DATA_T1 = 0;  // Temperature 1
float State_Data::DATA_T2 = 0;  // Temperature 2
float State_Data::DATA_T3 = 0;  // Temperature 3
float State_Data::DATA_THR = 0; // Thrust / Force
unsigned char State_Data::NEW_DATA = 0;
unsigned int State_Data::MODE = 0;