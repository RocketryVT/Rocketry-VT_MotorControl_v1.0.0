#include "Device-Sketch.h"

//#define XBee Serial1
#include <Adafruit_MotorShield.h>

//#include <SoftwareSerial.h> // XBee library
#include "Control.h"
#include "XBee_IO.h"
#include "Hardware.h"
#include "Adafruit_MAX31855.h"

/**
 * Rocket Motor Control Firmware
 * Outputs raw data and has ability to simulate data by receiving correct command
 */


/************ Function Declarations ************/

/**
 * Setup, runs before anything else
 * 
 * INPUT
 * void
 * 
 * RETURN
 * void
 */
void setup() {
  // Baud rates
  Serial.begin(baud);
  XBee.begin(baud);
/**
 * temp
 
 //while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  //Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  //delay(500);
uint8_t MAXDO  = 2;
uint8_t MAXCS  = 3;
uint8_t MAXCLK = 4;

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

while (1)
{

  unsigned long t1 = micros();
  //XBee.end();
  float tt = thermocouple.readCelsius();
  //XBee.begin(baud);
  t1 = micros() - t1;

  Serial.print("Temp: ");
  Serial.println(tt);
  Serial.print("Time: ");
  Serial.println(t1);
  Serial.println(" ");
  delay(1000);
}
*/
  Hardware::initializeStepperMotor();
  reset();
}

/**
 * Executes and repeats indefinietly or until the reset command is sent
 * Reset is accomplised by returning from the function and doing an iteration in loop()
 * 
 * INPUT
 * void
 * 
 * RETURN
 * void
 */
void loop() {

  using namespace XBeeIO;
  using namespace Hardware;

  unsigned long t;
  while (1) {
    t = micros();
   //NEW_DATA = update_data(); //temp
    // Listen for input commands  
    update_input_buffer();
    parse_input_buffer();

    /* MODE states */
    // 0 - Waiting
    // 1 - Armed
    // 2 - Starting
    // 3 - Firing
    // 4 - Stopped
    // 5 - Simulation
    if (0 < MODE && MODE < 4) {
      // Data Collection
      NEW_DATA = update_data();
      transmit_data(DATA_OUT_TYPE);
      //saveDataToSD();
      NEW_DATA = 0;
    }
  
    // Time to delay
    unsigned long t_delta_us = micros() - t;
    unsigned long t_wait = data_period_ms - ((t_delta_us/1000) % data_period_ms);
    delay( t_wait );
  }
}

void reset() {
  XBeeIO::clear_input_buffer();
  delay(data_period_ms);
  XBeeIO::transmit_data(0x00);
  MODE = 0;
  Hardware::initializeSaveFile();
  START_TIME = micros();
}


