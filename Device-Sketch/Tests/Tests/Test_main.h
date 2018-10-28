#pragma once

/** 
 * Function that drives unit test suite.
 *
 * INPUTS
 * bool istestondevice - Logical Flag, default to 1
 *						 1: Run the unit test for Arduino device (output through serial port)
 *						 0: Run the unit test for Desktop (output through command line)
 */
void Test_main(bool istestondevice = 1);