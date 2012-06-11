/*
 * ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy.h
 *
 *  Created on: Jan 12, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDEPTHFIRSTBESTSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDEPTHFIRSTBESTSTRATEGY_H_

#include "pheet/pheet.h"
#include "pheet/sched/strategies/UserDefinedPriority/UserDefinedPriority.h"

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy {
public:
	ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy();
	~ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy();

	UserDefinedPriority<Pheet> operator()(SubProblem* sub_problem, size_t* upper_bound);

	static void print_name();
};

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy() {

}

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy<Pheet, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy() {

}

template <class Pheet, class SubProblem>
UserDefinedPriority<Pheet> ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy<Pheet, SubProblem>::operator()(SubProblem* sub_problem, size_t* upper_bound) {
	size_t ubc = *upper_bound;
	size_t lb = sub_problem->get_lower_bound();
	if(ubc < lb) {
		// Prioritize this task, as it will immediatly terminate anyway
		// Do not allow it to be stolen (not worth it)
		return UserDefinedPriority<Pheet>(std::numeric_limits< prio_t >::max(), 0);
	}

	// Depth: assumption: good for pop, bad for steal
	size_t depth = sub_problem->sets[0].count() + sub_problem->sets[1].count();
	size_t max_depth = sub_problem->size;

	prio_t prio_pop = (std::numeric_limits< prio_t >::max() / (max_depth + 1)) * depth - lb;
	prio_t prio_steal = (std::numeric_limits< prio_t >::max() / (max_depth + 1)) * (max_depth - depth) - lb;

	return UserDefinedPriority<Pheet>(prio_pop, prio_steal);
}

template <class Pheet, class SubProblem>
inline void ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy<Pheet, SubProblem>::print_name() {
	std::cout << "DepthFirstBestStrategy";
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDEPTHFIRSTBESTSTRATEGY_H_ */
