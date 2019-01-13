#include "config.h"

/* XBee RX, TX pins */
// Connect DIN to pin 18 ,and DOUT to pin 19

std::string cfg::version = "MOTOR CONTROL 1.0.0";

/* Baud Rates */
unsigned long cfg::XBEE_BAUD = 38400;
unsigned long cfg::SD_BAUD = 115200;

/* Data Transmission Type */
unsigned int cfg::DATA_OUT_TYPE = -1; // x40;

std::chrono::time_point<std::chrono::steady_clock>
    cfg::TIME = std::chrono::steady_clock::now(),
    cfg::START_TIME = cfg::TIME,
    cfg::DATA_TIME = cfg::TIME;

std::chrono::milliseconds
    /* Device interface control variables */
    cfg::loop_period = std::chrono::milliseconds(50),
    cfg::xbee_write_period = std::chrono::milliseconds(1000),
    cfg::checkbuffer_period = std::chrono::milliseconds(100),
    /* Data Timing */
    cfg::pressure_period = std::chrono::milliseconds(10),
    cfg::temperature_period = std::chrono::milliseconds(100),
    cfg::loadcell_period = std::chrono::milliseconds(10);

/**************************** PINS ****************************/

/* Pressure Transducer Pins */
unsigned int cfg::pin_P1 = 1; // A1; /* Analog pin for oxidizer transducer*/
unsigned int cfg::pin_P2 = 2; // A2; /* Analog pin for c.chamber transducer*/

/* Thermocouple Pins*/
unsigned int cfg::pin_T1_DO = 2;
unsigned int cfg::pin_T1_CS = 3;
unsigned int cfg::pin_T1_CLK = 4;
unsigned int cfg::pin_T2_DO = 5;
unsigned int cfg::pin_T2_CS = 6;
unsigned int cfg::pin_T2_CLK = 7;
unsigned int cfg::pin_T3_DO = 8;
unsigned int cfg::pin_T3_CS = 9;
unsigned int cfg::pin_T3_CLK = 10;

/* Load Cell Pins */
unsigned int cfg::LOADCELL_DOUT = 11;
unsigned int cfg::LOADCELL_CLK = 12;
float cfg::loadcell_calibration_factor = -10000.0;

/* LED pin number */
unsigned int cfg::pin_LED = 7;

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

/* Data Timing Control */
// these initialize to the epoch of the clock being used
std::chrono::time_point<std::chrono::steady_clock>
    State_Data::last_pressure_time,
    State_Data::last_temperature_time,
    State_Data::last_loadcell_time;
