/*
 * ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters.h
 *
 *  Created on: Jan 12, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGSUBPROBLEMPERFORMANCECOUNTERS_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGSUBPROBLEMPERFORMANCECOUNTERS_H_

#include "pheet/pheet.h"

#include "pheet/primitives/PerformanceCounter/Basic/BasicPerformanceCounter.h"
#include "pheet/primitives/PerformanceCounter/Time/TimePerformanceCounter.h"

namespace pheet {

template <class Pheet>
class ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters {
public:
	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters();
	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters(ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>& other);
	~ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters();

	static void print_headers();
	void print_values();

	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_upper_bound_changes> num_upper_bound_changes;
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_allocated_subproblems> num_allocated_subproblems;

	TimePerformanceCounter<Pheet, graph_bipartitioning_test_measure_memory_allocation_time> memory_allocation_time;
};


template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters()
{

}

template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters(ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>& other)
:num_upper_bound_changes(other.num_upper_bound_changes),
 num_allocated_subproblems(other.num_allocated_subproblems),
 memory_allocation_time(other.memory_allocation_time)
{

}

template <class Pheet>
inline ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::~ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters() {

}

template <class Pheet>
inline void ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::print_headers() {
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_upper_bound_changes>::print_header("num_upper_bound_changes\t");
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_allocated_subproblems>::print_header("num_allocated_subproblems\t");
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_measure_memory_allocation_time>::print_header("memory_allocation_time\t");
}

template <class Pheet>
inline void ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::print_values() {
	num_upper_bound_changes.print("%d\t");
	num_allocated_subproblems.print("%d\t");
	memory_allocation_time.print("%f\t");
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGSUBPROBLEMPERFORMANCECOUNTERS_H_ */
