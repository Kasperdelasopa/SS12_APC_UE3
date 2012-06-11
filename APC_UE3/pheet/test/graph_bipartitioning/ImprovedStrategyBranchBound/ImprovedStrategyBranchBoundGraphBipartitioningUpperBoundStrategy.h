/*
 * ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy.h
 *
 *  Created on: Jan 13, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDSTRATEGY_H_

#include <pheet/pheet.h>
#include <pheet/sched/strategies/UserDefinedPriority/UserDefinedPriority.h>

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy {
public:
	ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy();
	~ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy();

	UserDefinedPriority<Pheet> operator()(SubProblem* sub_problem, size_t* upper_bound);

	static void print_name();
};

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy() {

}

template <class Pheet, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy<Pheet, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy() {

}

template <class Pheet, class SubProblem>
UserDefinedPriority<Pheet> ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy<Pheet, SubProblem>::operator()(SubProblem* sub_problem, size_t* upper_bound) {
	size_t lb = sub_problem->get_lower_bound();
	size_t ub = sub_problem->get_upper_bound();

	pheet_assert(ub >= lb);

	// pop task with lowest upper bound (often depth first, but not in extreme cases
	// (upper bound is "best guaranteed upper bound for this subtree" doesn't mean
	// that all branches will be below it!)
	prio_t prio_pop = std::numeric_limits< prio_t >::max() - ub;

	// steal task with highest range of uncertainty
	prio_t prio_steal = ub - lb;

	return UserDefinedPriority<Pheet>(prio_pop, prio_steal);
}

template <class Pheet, class SubProblem>
inline void ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy<Pheet, SubProblem>::print_name() {
	std::cout << "UpperBoundStrategy";
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDSTRATEGY_H_ */
