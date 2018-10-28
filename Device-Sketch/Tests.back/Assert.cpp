#include "stdafx.h"

#include "Assert.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>

/**
* Assertion function, with associated exception that it should throw
* This can be used in essentially the same way as the "assert" function in
* MATLAB and c11
*/
struct Test::Failed_Assertion_Exception
{
	int i;
	Failed_Assertion_Exception(int ii) { i = ii; }
};

/**
 * Sets verbosity level
 *
 * INPUTS
 * int x -> Verbosity level
 * 
 * OUTPUTS
 * bool -> Whether the change succeeded / was valid
 */
bool Test::set_DEBUG_VERBOSITY(int x)
{
	Test::DEBUG_VERBOSITY = x;
	return Test::DEBUG_VERBOSITY == x;
}

/**
* Notifies the user that the test has passed, because I'm lazy and don't want
* to #include <stdio.h> all the time. I expect the file name to be input or
* something manually so that the user knows what actually passed.
*
* INPUTS
bool arg -> Logical argument to test
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool Test::passed(const char* str)
{
	std::cout << GREEN_COLOR << "PASSED: " << DEFAULT_COLOR;
	std::cout << str << std::endl;
	return true;
}

/**
* Debugger message. Generates message with 0 tab at highest level, and adds
* a tab for each level below.
* TODO: Make this work with a static variable
*
* INPUTS
* bool arg -> Logical argument to test
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool Test::dmsg(const char* str, int priority)
{
	if (DEBUG_VERBOSITY == 0)
	{
		return false;
	}
	// Negate and take the unsigned value
	//unsigned int p = -priority;
	//unsigned int v = -DEBUG_VERBOSITY;

	if (priority <= DEBUG_VERBOSITY)
	{
		for (int i = 1; i < priority; i++)
		{
			std::cout << "\t";
		}
		std::cout << str << std::endl;
	}
	return true;

	//	if(verb > 1 && level == 1)
	//	{
	//		std::cout << "\t";
	//		std::cout << str << std::endl;
	//	}
	//	if(verb > 0 && level == 2)
	//	{
	//		std::cout << str << std::endl;
	//	}
	//
	//	return true;
}

/**
* Throws an exception and notifies the user that the assertion failed on the
* command line. It is left to the user to provide details of the failure and
* where it occured because I have no J-unit tests.
*
* INPUTS
* bool arg -> Logical argument to test
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool Test::assert_true(bool arg, const char* str, bool hard_fail)
{
	if (!arg)
	{
		std::cout << RED_COLOR << "ASSERTION FAILED: " << DEFAULT_COLOR;
		printf(str);
		printf("\n");
		if (hard_fail) throw Failed_Assertion_Exception(1);
	}
	return arg;
}

/**
* Throws an exception and notifies the user that the assertion failed on the
* command line. It is left to the user to provide details of the failure and
* where it occured because I have no J-unit tests.
*
* INPUTS
* long a -> First int a
* long a -> Second int b, expects this
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool Test::assert_equals(long a, long b, const char* str, bool hard_fail)
{
	if (a != b)
	{
		std::cout << RED_COLOR << "ASSERTION FAILED: " << DEFAULT_COLOR;
		std::cout << str;
		std::cout << "\n\t";
		std::cout << "Expected " << BLUE_COLOR << b << DEFAULT_COLOR \
			<< " but was " << BLUE_COLOR << a << DEFAULT_COLOR \
			<< std::endl;
		if (hard_fail) throw Failed_Assertion_Exception(1);
	}
	return a == b;
}

/**
* Throws an exception and notifies the user that the assertion failed on the
* command line. It is left to the user to provide details of the failure and
* where it occured because I have no J-unit tests.
*
* INPUTS
* double a -> First double argument
* double a -> Second double argument, expects this
* double prec -> Precision of equality
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool Test::assert_equals(double a, double b, double prec, const char* str, bool hard_fail)
{
	if (std::abs(a - b) > prec)
	{
		std::cout << RED_COLOR << "ASSERTION FAILED: " << DEFAULT_COLOR;
		std::cout << str;
		std::cout << "\n\t";
		std::cout << "Expected " << BLUE_COLOR << b << DEFAULT_COLOR \
			<< " but was " << BLUE_COLOR << a << DEFAULT_COLOR \
			<< std::endl;
		if (hard_fail) throw Failed_Assertion_Exception(1);
	}
	return (std::abs(a - b) <= prec);
}

