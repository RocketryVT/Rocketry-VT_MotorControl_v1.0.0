#include <iostream>

#include "Assert.h"
#include "Test_all.h"

int main()
{
	Test::run_tests(0);
    std::cout << Test::results_string << std::endl;
    return 0;
}
