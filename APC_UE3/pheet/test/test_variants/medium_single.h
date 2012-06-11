/*
 * medium_single.h
 *
 *  Created on: Nov 18, 2011
 *      Author: Martin Wimmer
 */

#ifndef MEDIUM_SINGLE_H_
#define MEDIUM_SINGLE_H_

#include "pheet/misc/types.h"

namespace pheet {

#define SORTING_TEST true
const procs_t sorting_test_cpus[] = {1, 2, 4, 8, 16, 32};
const unsigned int sorting_test_seeds[] = {0};
const size_t sorting_test_n[] = {100000000};
const int sorting_test_types[] = {0};

#define GRAPH_BIPARTITIONING_TEST true
const procs_t graph_bipartitioning_test_cpus[] = {1, 2, 4, 8, 16, 32};
const unsigned int graph_bipartitioning_test_seeds[] = {0};
const GraphBipartitioningProblem graph_bipartitioning_test_problems[] = {
		// n, p, max_w
		{35, 0.5, 1000}
};
/*
const size_t graph_bipartitioning_test_n[] = {27};
const double graph_bipartitioning_test_p[] = {0.5};
const double graph_bipartitioning_test_max_w[] = {1000};
*/
const int graph_bipartitioning_test_types[] = {0};

#define INAROW_TEST true
const procs_t inarow_test_cpus[] = {1, 2, 4, 8, 16, 32, 64, 128};
const unsigned int inarow_test_lookaheads[] = {7};

const bool    nqueens_test        = false;
const procs_t nqueens_test_cpus[] = {1, 2, 4, 8, 16, 32};
const size_t  nqueens_test_n[]    = {32};

}

#endif /* MEDIUM_SINGLE_H_ */
