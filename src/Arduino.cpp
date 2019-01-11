#include "stdafx.h"
#include <stdio.h>

unsigned long micros() {
	return 123456; // Dummy value, same value as what is tested and expected in MATLAB test cases
}

void print(const char* str)
{
	printf("%s", str);
}