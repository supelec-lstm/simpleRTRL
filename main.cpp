// Copyright PinaPL
//
// main.cpp
// PinaPL
//

#include <iostream>
#include <sys/time.h>

#include "test.h"


int main(int argc, const char * argv[]) {
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand((unsigned int)ts.tv_nsec);
	
	if (argc > 1)
		grammarLearn((*argv[1] != '0'));
	else
		grammarLearn(false);
	
	return 0;
}
