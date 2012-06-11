/*
* SORTests.h
*
*  Created on: 5.12.2011
*      Author: Daniel Cederman
*     License: Boost Software License 1.0 (BSL1.0)
*/


#ifndef SORTESTS_H_
#define SORTESTS_H_

#include "../Test.h"
#include "../../init.h"

namespace pheet {

	class SORTests
	{
	public:
		void run_test();
	private:
		template <class Scheduler>
		void test();
	};

}

#endif /* SORTESTS_H_ */
