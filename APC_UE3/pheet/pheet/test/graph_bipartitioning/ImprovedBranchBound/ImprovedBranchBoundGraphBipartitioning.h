/*
 * ImprovedImprovedBranchBoundGraphBipartitioning.h
 *
 *  Created on: Dec 2, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONING_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONING_H_

#include "../graph_helpers.h"
#include "ImprovedBranchBoundGraphBipartitioningSubproblem.h"
#include "ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic.h"
#include "ImprovedBranchBoundGraphBipartitioningPerformanceCounters.h"
#include "ImprovedBranchBoundGraphBipartitioningTask.h"

#include <iostream>

namespace pheet {

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize = 64>
class ImprovedBranchBoundGraphBipartitioningImpl {
public:
	typedef ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize> Self;
	typedef GraphBipartitioningSolution<MaxSize> Solution;
	typedef MaxReducer<Pheet, Solution> SolutionReducer;
	typedef ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet> PerformanceCounters;
	typedef ImprovedBranchBoundGraphBipartitioningSubproblem<Pheet, Logic, MaxSize> SubProblem;
	typedef ImprovedBranchBoundGraphBipartitioningTask<Pheet, Logic, MaxSize> BBTask;

	template <template <class P, class SP> class NewLogic>
		using WithLogic = ImprovedBranchBoundGraphBipartitioningImpl<Pheet, NewLogic, MaxSize>;

	template <size_t ms>
		using WithMaxSize = ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, ms>;

	ImprovedBranchBoundGraphBipartitioningImpl(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc);
	~ImprovedBranchBoundGraphBipartitioningImpl();

	void operator()();

	static void print_headers();
	static void print_configuration();

	static char const name[];

private:
	GraphVertex* data;
	size_t size;
	Solution& solution;
	ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet> pc;
};

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
char const ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::name[] = "ImprovedBranchBoundGraphBipartitioning";

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::ImprovedBranchBoundGraphBipartitioningImpl(GraphVertex* data, size_t size, Solution& solution, PerformanceCounters& pc)
: data(data), size(size), solution(solution), pc(pc) {

}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::~ImprovedBranchBoundGraphBipartitioningImpl() {

}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
void ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::operator()() {
	SolutionReducer best;

	size_t ub = std::numeric_limits< size_t >::max();

	size_t k = size >> 1;
	SubProblem* prob = new SubProblem(data, size, k);
	Pheet::template
		finish<BBTask>(prob, &ub, best, pc);

	solution = best.get_max();
	pheet_assert(solution.weight == ub);
	pheet_assert(ub != std::numeric_limits< size_t >::max());
	pheet_assert(solution.weight != std::numeric_limits< size_t >::max());
	pheet_assert(solution.sets[0].count() == k);
	pheet_assert(solution.sets[1].count() == size - k);
}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
void ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::print_configuration() {
	Logic<Pheet, SubProblem>::print_name();
	std::cout << "\t";
}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
void ImprovedBranchBoundGraphBipartitioningImpl<Pheet, Logic, MaxSize>::print_headers() {
	std::cout << "logic\t";
}

template <class Pheet>
using ImprovedBranchBoundGraphBipartitioning = ImprovedBranchBoundGraphBipartitioningImpl<Pheet, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic, 64>;


}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONING_H_ */
