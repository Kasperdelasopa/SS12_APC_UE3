/*
* UTSTests.cpp
*
*  Created on: 5.12.2011
*      Author: Daniel Cederman
*     License: Boost Software License 1.0 (BSL1.0)
*/

#include "UTSTests.h"
#include "UTSTest.h"
#include "RecursiveSearch/UTSRun.h"

#include "../test_schedulers.h"
#include <iostream>

namespace pheet {

	const std::string uts_test_standardworkloads_params[] = {"T1 -t 1 -a 3 -d 10 -b 4 -r 19",
												"T5 -t 1 -a 0 -d 20 -b 4 -r 34",
												"T2 -t 1 -a 2 -d 16 -b 6 -r 502",
												"T3 -t 0 -b 2000 -q 0.124875 -m 8 -r 42",
												"T4 -t 2 -a 0 -d 16 -b 6 -r 1 -q 0.234375 -m 4 -r 1",
												"T1L -t 1 -a 3 -d 13 -b 4 -r 29",
												"T2L -t 1 -a 2 -d 23 -b 7 -r 220",
												"T3L -t 0 -b 2000 -q 0.200014 -m 5 -r 7",
												"T1XL -t 1 -a 3 -d 15 -b 4 -r 29"};

	template <class Test>
	void UTSTests::test()
	{
#ifdef UTS_TEST
		for(size_t s = 0; s < sizeof(uts_test_standardworkloads)/sizeof(uts_test_standardworkloads[0]); s++) {
			for(size_t c = 0; c < sizeof(inarow_test_cpus)/sizeof(inarow_test_cpus[0]); c++) {
				UTSTest<Test> iart(uts_test_cpus[c],uts_test_standardworkloads_params[uts_test_standardworkloads[s]]);
				iart.run_test();
			}
		}
#endif
	}

	void UTSTests::run_test()
	{
#ifdef UTS_TEST
		if(uts_test) {
			test<UTSRun<PrimitiveHeapPriorityScheduler> >();
		}
#endif
	}

}
