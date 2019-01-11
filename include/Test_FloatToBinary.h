#ifndef TEST_FLOAT_TO_BINARY_H
#define TEST_FLOAT_TO_BINARY_H

/**
 * So uh, maybe this will work and maybe it won't. The float may be defined
 * differently on Arduino than on Windows. Using these conversions though, I
 * can bypass this a little to make sure that the values are converted correctly
 * on the serial interface.
 *
 * Update: 18/04/29
 * Arduino, Matlab, and Visual C++ use IEEE 754 standard 32 bit float points,
 * this method works
 */
int TestFloatConversions();

#endif // TEST_FLOAT_TO_BINARY_H
