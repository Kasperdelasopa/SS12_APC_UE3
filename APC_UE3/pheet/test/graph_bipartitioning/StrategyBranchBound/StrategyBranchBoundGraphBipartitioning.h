/*
 * StrategyBranchBoundGraphBipartitioning.h
 *
 *  Created on: 08.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef STRATEGYBRANCHBOUNDGRAPHBIPARTITIONING_H_
#define STRATEGYBRANCHBOUNDGRAPHBIPARTITIONING_H_

#include "../graph_helpers.h"
#include "StrategyBranchBoundGraphBipartitioningTask.h"
#include "StrategyBranchBoundGraphBipartitioningPerformanceCounters.h"

#include "../BranchBound/BasicLowerBound.h"
#include "../BranchBound/BasicNextVertex.h"
#include "BranchBoundGraphBipartitioningBestFirstStrategy.h"

#include <iostream>

/*
 *
 */
namespace pheet {

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize = 64>
class StrategyBranchBoundGraphBipartitioningImpl {
public:
	typedef StrategyBranchBoundGraphBipartitioning<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize> Self;
	typedef GraphBipartitioningSolution<MaxSize> Solution;
	typedef MaxReducer<Pheet, Solution> SolutionReducer;
	typedef StrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet> PerformanceCounters;
	typedef StrategyBranchBoundGraphBipartitioningTask<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize> BBTask;

	StrategyBranchBoundGraphBipartitioningImpl(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc);
	~StrategyBranchBoundGraphBipartitioningImpl();

	void operator()();

	static void print_configuration();
	static void print_headers();

	static char const name[];

private:
	GraphVertex* data;
	size_t size;
	GraphBipartitioningSolution<MaxSize> solution;
	PerformanceCounters pc;
};

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
char const StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::name[] = "StrategyBranchBoundGraphBipartitioning";

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::StrategyBranchBoundGraphBipartitioningImpl(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc)
: data(data), size(size), solution(solution), pc(pc) {

}

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::~StrategyBranchBoundGraphBipartitioningImpl() {

}

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
void StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::partition() {
	SolutionReducer best;
	size_t ub = std::numeric_limits< size_t >::max();

	size_t k = size >> 1;
	Pheet::template finish<BBTask>(graph, size, k, best, static_cast<size_t*>(new size_t[k]), 0, static_cast<size_t*>(new size_t[size - k]), 0, &ub, 0);

	solution = best.get_max();
}

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
void StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::print_configuration() {
	std::cout << LowerBound::name << "\t" << NextVertex::name << "\t";
	SchedulingStrategy<Pheet>::print_name();
	std::cout << "\t";
}

template <class Pheet, class LowerBound, class NextVertex, template <class P> class SchedulingStrategy, size_t MaxSize>
void StrategyBranchBoundGraphBipartitioningImpl<Pheet, LowerBound, NextVertex, SchedulingStrategy, MaxSize>::print_headers() {
	std::cout << "lower_bound\tnext_vertex\tstrategy\t";
}

template <class Pheet>
using StrategyBranchBoundGraphBipartitioning = StrategyBranchBoundGraphBipartitioningImpl<Pheet, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy, 64>;

}

#endif /* STRATEGYBRANCHBOUNDGRAPHBIPARTITIONING_H_ */
