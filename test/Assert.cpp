#include <cmath>

#include "Assert.h"
#include "config.h"

namespace Test
{

std::string results_string;

/**
* Assertion function, with associated exception that it should throw
* This can be used in essentially the same way as the "assert" function in
* MATLAB
*/
struct Failed_Assertion_Exception
{
	int i;
	Failed_Assertion_Exception(int ii) { i = ii; }
};

// Notifies the user that the test has passed
void passed(const std::string& str)
{
    results_string += "<P>:" + str + '\n';
}

// Notifies the user that the test has failed
void failed(const std::string& str)
{
	results_string += "<F>:" + str + '\n';
}

// adds debugger message to results_string
void dmsg(const std::string& str)
{
    results_string += str + '\n';
}

/**
* Throws an exception and notifies the user that the assertion failed on the
* command line. It is left to the user to provide details of the failure and
* where it occured because I have no J-unit tests.
*
* INPUTS
* bool arg -> Logical argument to test
* const char* str -> Error message
* bool soft -> Soft fail flag, use false to keep
* from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool assert_true(bool arg, const std::string& str, bool hard_fail)
{
	if (!arg) results_string += "<!>:" + str + '\n';
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
* bool soft -> Soft fail flag, use false
* to keep from throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool assert_equals(long a, long b, const std::string& str, bool hard_fail)
{
    return assert_true(a == b, str, hard_fail);
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
* bool soft -> Soft fail flag, use false to keep from
* throwing an exception on failure
*
* OUTPUTS
* bool -> The result of the input logical argument
*/
bool assert_equals(double a, double b, double prec,
    const std::string &str, bool hard_fail)
{
    return assert_true(std::abs(a - b) < prec, str, hard_fail);
}

} // namespace Test

