#include "control_main.h"
/**
 * Rocket Motor Control Firmware
 * Outputs raw data and has ability to simulate data by receiving correct command
 */

/**
 * Setup, runs before anything else
 */
void setup() {
	control::setup(); 
}

/**
 * Executes and repeats indefinietly or until the reset command is sent
 * Reset is accomplised by returning from the function and doing an iteration in loop()
 */
void loop() {
	control::loop();
}


