/*
 * ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategy.h
 *
 *  Created on: Dec 5, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGAUTOSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGAUTOSTRATEGY_H_

#include <pheet/pheet.h>
#include "pheet/sched/strategies/LifoFifo/LifoFifoStrategy.h"

namespace pheet {

template <class Pheet, template <class P> class Strategy, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl {
public:
	template <template <class P> class NewStrat>
	using WithStrategy = ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, NewStrat, SubProblem>;

	ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl();
	~ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl();

	Strategy<Pheet> operator()(SubProblem* sub_problem, size_t* upper_bound);

	static void print_name();
};

template <class Pheet, template <class P> class Strategy, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl() {

}

template <class Pheet, template <class P> class Strategy, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl() {

}

template <class Pheet, template <class P> class Strategy, class SubProblem>
inline Strategy<Pheet> ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy, SubProblem>::operator()(SubProblem* sub_problem, size_t* upper_bound) {
	return Strategy<Pheet>();
}

template <class Pheet, template <class P> class Strategy, class SubProblem>
void ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy, SubProblem>::print_name() {
	std::cout << "AutoStrategy<";
	Strategy<Pheet>::print_name();
	std::cout << ">";
}

template <class Pheet, class SubProblem>
using ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategy = ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, LifoFifoStrategy, SubProblem>;

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGAUTOSTRATEGY_H_ */
