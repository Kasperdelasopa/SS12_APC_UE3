/*
 * ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy.h
 *
 *  Created on: Jan 13, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDFIFOSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDFIFOSTRATEGY_H_

#include <pheet/pheet.h>
#include <pheet/sched/strategies/BaseStrategy.h>

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy : public BaseStrategy<Pheet> {
public:
	typedef ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem> Self;
	ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(SubProblem* sub_problem, size_t* upper_bound);
	ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(Self const& other);
	ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(Self const&& other);
	~ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy();

	virtual prio_t get_pop_priority(size_t task_id);
	virtual prio_t get_steal_priority(size_t task_id, typename Pheet::Environment::StealerDescriptor& desc);
	virtual BaseStrategy<Pheet>* clone();

	static void print_name();

private:
	prio_t pop_prio;
};

template <class Pheet, class SubProblem>
ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(SubProblem* sub_problem, size_t* upper_bound)
:pop_prio(std::numeric_limits< prio_t >::max() - sub_problem->get_upper_bound())
{

}

template <class Pheet, class SubProblem>
ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(Self const& other)
:pop_prio(other.pop_prio)
{

}

template <class Pheet, class SubProblem>
ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy(Self const&& other)
:pop_prio(other.pop_prio)
{

}

template <class Pheet, class SubProblem>
ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy() {

}
template <class Pheet, class SubProblem>
inline prio_t ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::get_pop_priority(size_t task_id) {
	return pop_prio;
}

template <class Pheet, class SubProblem>
inline prio_t ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::get_steal_priority(size_t task_id, typename Pheet::Environment::StealerDescriptor& desc) {
	return std::numeric_limits< prio_t >::max() - task_id;
}

template <class Pheet, class SubProblem>
inline BaseStrategy<Pheet>* ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::clone() {
	return new Self(*this);
}

template <class Pheet, class SubProblem>
inline void ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy<Pheet, SubProblem>::print_name() {
	std::cout << "UpperBoundFifoStrategy";
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGUPPERBOUNDFIFOSTRATEGY_H_ */
