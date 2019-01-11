#ifndef ASSERT_H
#define ASSERT_H

#include "Default_Config.h"

namespace Test
{
	/** Test Results String */
	extern char resstr[test_str_length];
	extern unsigned int resstriter;
	
	/**
	 * Returns the absolute value of a double
	 */
	double absolute(double n);

	/**
	* Assertion function, with associated exception that it should throw
	* This can be used in essentially the same way as the "assert" function in
	* MATLAB and c11
	*/
	struct Failed_Assertion_Exception;

	/**
	* Notifies the user that the test has passed
	*
	* INPUTS
	* const char* str -> Output message
	*
	* OUTPUTS
	* bool -> The result of the input logical argument
	*/
	bool passed(const char* str);

	/**
	* Notifies the user that the test has failed
	*
	* INPUTS
	* const char* str -> Output message
	*
	* OUTPUTS
	* bool -> The result of the input logical argument
	*/
	bool failed(const char* str);

	/**
	* Debugger message. Saves string to the resstr string for output later
	*
	* INPUTS
	* const char* str -> Error message
	*
	* OUTPUTS
	* bool -> The result of the input logical argument
	*/
	bool dmsg(const char* str);

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

#endif // ASSERT_H
