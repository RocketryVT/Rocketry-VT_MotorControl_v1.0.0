#pragma once

#include <string>

namespace Test
{
	/**
	* Static colors
	*/
	static const std::string RED_COLOR = "";// "\33[32;91m";
	static const std::string GREEN_COLOR = "";// "\33[32;32m";
	static const std::string BLUE_COLOR = "";// "\33[32;36m";
	static const std::string DEFAULT_COLOR = "";// "\33[0m";

	/**
	 * Test library verbosity level
	 */
	static int DEBUG_VERBOSITY = 0;

	/**
	* Sets verbosity level
	*/
	bool set_DEBUG_VERBOSITY(int x);

	/**
	* Assertion function, with associated exception that it should throw
	* This can be used in essentially the same way as the "assert" function in
	* MATLAB and c11
	*/
	struct Failed_Assertion_Exception;

	/**
	* Notifies the user that the test has passed, because I'm lazy and don't want
	* to #include <stdio.h> all the time. I expect the file name to be input or
	* something manually so that the user knows what actually passed.
	*
	* INPUTS
	* bool arg -> Logical argument to test
	* const char* str -> Error message
	* bool soft -> Soft fail flag, use false to keep from throwing an exception on failure
	*
	* OUTPUTS
	* bool -> The result of the input logical argument
	*/
	bool passed(const char* str);

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
	bool dmsg(const char* str, int priority);

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
	bool assert_true(bool arg, const char* str = 0, bool hard_fail = false);

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
	bool assert_equals(long a, long b, const char* str = 0, bool hard_fail = false);

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
	bool assert_equals(double a, double b, double prec, const char* str = 0, bool hard_fail = false);

} // end namespace test
