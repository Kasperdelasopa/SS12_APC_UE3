/*
 * ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters.h
 *
 *  Created on: Jan 12, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_

#include "pheet/pheet.h"

#include "pheet/primitives/PerformanceCounter/Basic/BasicPerformanceCounter.h"
#include "../ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters.h"

namespace pheet {

template <class Pheet>
class ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters {
public:
	ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters();
	ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters(ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters& other);
	ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters(ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters&& other);
	~ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters();

	static void print_headers();
	void print_values();

	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_irrelevant_tasks> num_irrelevant_tasks;
	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet> subproblem_pc;
};


template <class Pheet>
inline ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters()
{

}

template <class Pheet>
inline ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters(ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>& other)
:num_irrelevant_tasks(other.num_irrelevant_tasks),
 subproblem_pc(other.subproblem_pc)
{

}

template <class Pheet>
inline ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters(ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>&& other)
:num_irrelevant_tasks(other.num_irrelevant_tasks),
 subproblem_pc(other.subproblem_pc)
{

}

template <class Pheet>
inline ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::~ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters() {

}

template <class Pheet>
inline void ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::print_headers() {
	BasicPerformanceCounter<Pheet, graph_bipartitioning_test_count_irrelevant_tasks>::print_header("num_irrelevant_tasks\t");

	ImprovedBranchBoundGraphBipartitioningSubproblemPerformanceCounters<Pheet>::print_headers();
}

template <class Pheet>
inline void ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet>::print_values() {
	num_irrelevant_tasks.print("%d\t");

	subproblem_pc.print_values();
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_ */
