/*
 * pheet_tests.cpp
 *
 *  Created on: 07.04.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#define NDEBUG 1

#include "init.h"
#include "sorting/SortingTests.h"
#include "graph_bipartitioning/GraphBipartitioningTests.h"
#include "lupiv/LUPivTests.h"
/*
#include "inarow/InARowTests.h"
#include "n-queens/NQueensTests.h"
#include "lupiv/LUPivTests.h"
#include "uts/UTSTests.h"
#include "sor/SORTests.h"*/

using namespace pheet;

int main(int argc, char* argv[]) {
  	SortingTests st;
  	st.run_test();

  	GraphBipartitioningTests gpt;
  	gpt.run_test();
/*
	InARowTests iarts;
	iarts.run_test();

	NQueensTests nqt;
	nqt.run_test();
*/
	LUPivTests lpt;
	lpt.run_test();
/*
	UTSTests utss;
	utss.run_test();

	SORTests sors;
	sors.run_test();*/

	return 0;
}
