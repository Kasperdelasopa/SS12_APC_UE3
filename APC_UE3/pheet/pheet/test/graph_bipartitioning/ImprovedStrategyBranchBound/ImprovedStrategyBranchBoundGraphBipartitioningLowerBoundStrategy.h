/*
 * ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy.h
 *
 *  Created on: Dec 21, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGLOWERBOUNDSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGLOWERBOUNDSTRATEGY_H_

#include <pheet/pheet.h>
#include <pheet/sched/strategies/UserDefinedPriority/UserDefinedPriority.h>

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy {
public:
	ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy();
	~ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy();

	UserDefinedPriority<Pheet> operator()(SubProblem* sub_problem, size_t* upper_bound);

	static void print_name();
};

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy() {

}

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy<Pheet, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy() {

}

template <class Pheet, class SubProblem>
UserDefinedPriority<Pheet> ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy<Pheet, SubProblem>::operator()(SubProblem* sub_problem, size_t* upper_bound) {
	size_t lb = sub_problem->get_lower_bound();
	size_t ub = *upper_bound;

/*	prio_t prio_pop = 1 + depth * bound_diff;
	prio_t prio_steal = 1 + (sub_problem->size - depth) * bound_diff;*/

	return UserDefinedPriority<Pheet>(ub - lb, ub - lb);
}

template <class Pheet, class SubProblem>
inline void ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy<Pheet, SubProblem>::print_name() {
	std::cout << "LowerBoundStrategy";
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGLOWERBOUNDSTRATEGY_H_ */
