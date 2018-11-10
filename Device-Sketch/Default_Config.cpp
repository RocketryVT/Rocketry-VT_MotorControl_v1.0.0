
#include "stdafx.h"
#include "Default_Config.h"
#include "Arduino.h"

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

/* Device interface control variables */
unsigned long Default_Config::TIME = 0;
unsigned long Default_Config::data_period_ms = 10;//10;
unsigned long Default_Config::XBeeWrite_period_ms = 1000;//100;
unsigned long Default_Config::SDWrite_period_ms = 250;//100;
unsigned long Default_Config::checkbuffer_period_ms = 10;//100;
unsigned long Default_Config::max_time = 30;
unsigned long Default_Config::XBEE_BAUD = 38400;
unsigned long Default_Config::SD_BAUD = 115200;
unsigned int Default_Config::DATA_OUT_TYPE = 0x51;
unsigned long Default_Config::START_TIME = 0;

/**************************** PINS ****************************/

/* Pressure Transducer Pins */
unsigned int Pins_Config::pin_P1 = A1; /* Analog pin for oxidizer transducer*/
unsigned int Pins_Config::pin_P2 = A2; /* Analog pin for c.chamber transducer*/

/* Thermocouple Pins*/
unsigned int Pins_Config::pin_T1_DO = 2;
unsigned int Pins_Config::pin_T1_CS = 3;
unsigned int Pins_Config::pin_T1_CLK = 4;
unsigned int Pins_Config::pin_T2_DO = 5;
unsigned int Pins_Config::pin_T2_CS = 6;
unsigned int Pins_Config::pin_T2_CLK = 7;
unsigned int Pins_Config::pin_T3_DO = 8;
unsigned int Pins_Config::pin_T3_CS = 9;
unsigned int Pins_Config::pin_T3_CLK = 10;

/* Load Cell Pins */
unsigned int Pins_Config::LOADCELL_DOUT = 11;
unsigned int Pins_Config::LOADCELL_CLK = 12;
float Pins_Config::loadcell_calibration_factor = -10000.0;

/* LED pin number */
unsigned int Pins_Config::pin_LED = 7;

/**************************** DATA ****************************/

/* Initialize Data variables*/
int State_Data::STATUS = 0;
float State_Data::DATA_P1 = 0;  // Pressure 1
float State_Data::DATA_P2 = 0;  // Pressure 3
float State_Data::DATA_T1 = 0;  // Temperature 1
float State_Data::DATA_T2 = 0;  // Temperature 2
float State_Data::DATA_T3 = 0;  // Temperature 3
float State_Data::DATA_THR = 0; // Thrust / Force
unsigned char State_Data::NEW_DATA = 0;
unsigned int State_Data::MODE = 0;