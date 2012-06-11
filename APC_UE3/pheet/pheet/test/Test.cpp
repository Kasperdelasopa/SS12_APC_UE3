/*
 * Test.cpp
 *
 *  Created on: 07.04.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#include "Test.h"

namespace pheet {

Test::Test() {
}

Test::~Test() {
}

void Test::check_time(Time & time) {
	gettimeofday(&time, NULL);
}

double Test::calculate_seconds(Time const& start, Time const& end) {
	double dStart = 1.0e-6 * start.tv_usec;
	double dStop = (end.tv_sec - start.tv_sec) + 1.0e-6 * end.tv_usec;

	return dStop - dStart;
}
}
