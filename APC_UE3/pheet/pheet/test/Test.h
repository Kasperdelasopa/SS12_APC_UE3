/*
 * Test.h
 *
 *  Created on: 07.04.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef TEST_H_
#define TEST_H_

#include "init.h"

#include <sys/time.h>
#include <stdlib.h>

namespace pheet {

class Test {
public:
	typedef struct timeval Time;

	Test();
	virtual ~Test();

	virtual void run_test() = 0;

	void check_time(Time& time);
	double calculate_seconds(Time const& start, Time const& end);
};

}


#endif /* TEST_H_ */
