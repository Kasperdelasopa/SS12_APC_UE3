/*
 * BranchBoundGraphBipartitioning.h
 *
 *  Created on: 08.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef BRANCHBOUNDGRAPHBIPARTITIONING_H_
#define BRANCHBOUNDGRAPHBIPARTITIONING_H_

#include "../graph_helpers.h"
#include "BranchBoundGraphBipartitioningTask.h"
#include "BranchBoundGraphBipartitioningPerformanceCounters.h"

#include <iostream>

/*
 *
 */
namespace pheet {

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize = 64>
class BranchBoundGraphBipartitioning {
public:
	typedef BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize> Self;
	typedef GraphBipartitioningSolution<MaxSize> Solution;
	typedef MaxReducer<Pheet, Solution> SolutionReducer;
	typedef BranchBoundGraphBipartitioningPerformanceCounters<Pheet> PerformanceCounters;
	typedef BranchBoundGraphBipartitioningTask<Pheet, LowerBound, NextVertex, MaxSize> BBTask;

	BranchBoundGraphBipartitioning(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc);
	~BranchBoundGraphBipartitioning();

	void operator()();

	static void print_configuration();
	static void print_headers();

	static char const name[];

private:
	GraphVertex* data;
	size_t size;
	GraphBipartitioningSolution<MaxSize>& solution;
	PerformanceCounters pc;
};

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
procs_t const BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::max_cpus = Pheet::max_cpus;

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
char const BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::name[] = "BranchBoundGraphBipartitioning";

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
char const * const BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::scheduler_name = Pheet::name;

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::BranchBoundGraphBipartitioning(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc)
: data(data), size(size), solution(solution), pc(pc) {

}

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::~BranchBoundGraphBipartitioning() {

}

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
void BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::operator()() {
	SolutionReducer best;
	size_t ub = std::numeric_limits< size_t >::max();

	size_t k = size >> 1;
	Pheet::template
		finish<BBTask>(graph, size, k, best, static_cast<size_t*>(new size_t[k]), 0, static_cast<size_t*>(new size_t[size - k]), 0, &ub, 0);

	solution = best.get_max();
}

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
void BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::print_configuration() {
	std::cout << LowerBound::name << "\t" << NextVertex::name << "\t";
}

template <class Pheet, class LowerBound, class NextVertex, size_t MaxSize>
void BranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, MaxSize>::print_headers() {
	std::cout << "lower_bound\tnext_vertex\t";
}

}

#endif /* BRANCHBOUNDGRAPHBIPARTITIONING_H_ */
