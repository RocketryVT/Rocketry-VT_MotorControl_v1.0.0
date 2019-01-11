#include <iostream>

#include "Assert.h"
#include "Test_all.h"

int main()
{
	run_tests(0);
    std::cout << Test::resstr << std::endl;
    return 0;
}
