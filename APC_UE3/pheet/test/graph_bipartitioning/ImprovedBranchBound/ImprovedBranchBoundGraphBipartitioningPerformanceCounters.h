/*
 * ImprovedBranchBoundGraphBipartitioningPerformanceCounters.h
 *
 *  Created on: Jan 12, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_

#include <pheet/pheet.h>

#include <pheet/primitives/PerformanceCounter/Basic/BasicPerformanceCounter.h>
#include "ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters.h"

namespace pheet {

template <class Pheet>
class ImprovedBranchBoundGraphBipartitioningPerformanceCounters {
public:
	typedef ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet> Self;

	ImprovedBranchBoundGraphBipartitioningPerformanceCounters();
	ImprovedBranchBoundGraphBipartitioningPerformanceCounters(Self& other);
	~ImprovedBranchBoundGraphBipartitioningPerformanceCounters();

	static void print_headers();
	void print_values();

	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_irrelevant_tasks> num_irrelevant_tasks;
	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet> subproblem_pc;
};

template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::ImprovedBranchBoundGraphBipartitioningPerformanceCounters()
{

}

template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::ImprovedBranchBoundGraphBipartitioningPerformanceCounters(Self& other)
:num_irrelevant_tasks(other.num_irrelevant_tasks),
 subproblem_pc(other.subproblem_pc)
{

}

template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::~ImprovedBranchBoundGraphBipartitioningPerformanceCounters() {

}

template <class Pheet>
inline void ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::print_headers() {
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_irrelevant_tasks>::print_header("num_irrelevant_tasks\t");

	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::print_headers();
}

template <class Pheet>
inline void ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::print_values() {
	num_irrelevant_tasks.print("%d\t");

	subproblem_pc.print_values();
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_ */
